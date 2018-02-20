#include "uicontroller.h"

UIController::UIController(Arduino * arduino, QQmlApplicationEngine* engine, QObject * qml, QObject *parent) : QObject(parent)
{
    this->distance = 0;
    this->arduino = arduino;
    this->manager = this->arduino->getManager();
    this->engine = engine;
     QObject::connect(this, SIGNAL(newFinishDistance(int)), this->arduino->dist, SLOT(interrupt()));
     QObject::connect(this->arduino, SIGNAL(signalChoice(int)), this, SLOT(choiceDispatch(int)));
     QObject::connect(this->arduino->dist, SIGNAL(updateDistance(int)), this, SLOT(updateDistance(int)));
     QObject::connect(this->arduino->dist, SIGNAL(finished()), this,SLOT(start()));
     QObject::connect(this, SIGNAL(busy(QString)), this, SLOT(stop(QString)));
     QObject::connect(this, SIGNAL(free()), this, SLOT(start()));
     QObject::connect(this->arduino, SIGNAL(readyToUpdate()), this, SLOT(updateInfo()));
    this->qml = qml;
     this->startSerialLine();
}

UIController::~UIController(){}

QString UIController::getLoadingMsg(){
    return this->loadingMsg;
}

void UIController::startRacer(int bib){
    emit this->busy(QString("Ready!"));
    this->bib = bib;
    while(this->arduino->getMonitor()->isRunning()); //wait
    std::thread * thr = new std::thread([this] {
            this->arduino->startRacer(this->bib);
            emit this->free();
     });
}
void UIController::setLoadingMsg(QString msg){
    this->loadingMsg = msg;
}

void UIController::start(){
    this->startSerialLine();
    emit this->unlock();
}

void UIController::stop(QString reason){
    this->setLoadingMsg(reason);
    if(this->arduino->getMonitor()->isRunning()){
        this->stopSerialLine();
    }
    emit this->lock();
}

bool UIController::addDevice(int ID, int pos){
    emit this->busy(QString("Adding device"));
    this->tempDevice = new Device(ID, pos);
    while(this->arduino->getMonitor()->isRunning()); //wait
   // std::thread * thr = new std::thread([this] {
        this->arduino->addDevice(this->tempDevice->getID(), this->tempDevice->getPosition());
        this->arduino->sync();
    //});
   emit free();
}

void UIController::choiceDispatch(int choice){
    Packet * p = 0;
    if(choice == 1){
        emit busy(QString("Being added to network"));
         std::thread * thr = new std::thread([this] {
             Packet * p = 0;
            Device * device = new Device();
            if(this->arduino->addDevice(-1, -1, device)){
                 if(this->arduino->getManager()->connect(SERIAL)){
                    if(this->arduino->getManager()->send(1)){
                        while(this->arduino->getManager()->getCurrentConnection()->available() <= 0); //wait
                         int confirm= 0;
                         p = this->arduino->getManager()->read();
                         memcpy(&confirm, (int*) p->getData(), p->getSize());
                         this->updateInfo();
                        emit free();
                    }
                 }
              }
            }
       );
    }else if(choice == 2){
        emit busy(QString("Synchronizing"));
        std::thread * thr = new std::thread([this] {
            Packet * p = 0;
            if(this->manager->connect(SERIAL)){
                std::cout << "Synchronizing" << std::flush;
                for(int i = 0; this->manager->getCurrentConnection()->available() <= 0; i++){
                    if(i%100000 == 0)
                        std::cout << "." << std::flush;
                }
                int done = 0;
                p = this->manager->read();
                memcpy(&done, (int*)p->getData(), p->getSize());
                std::cout << std::endl << "Sync Complete!" << std::endl;
                emit free();
            }
        }
     );
    }else if(choice == 4){
        emit this->raceMode();
        this->goRaceMode();
    }
}

Racer * UIController::getLastRacer(){
   return  this->arduino->getTimeQueue()->head();
}

void UIController::goRaceMode(){
    std::thread * thr = new std::thread([this] {
        Packet * p = 0;
        uiCounter = false;
        std::thread * th;

        while(this->arduino->getMonitor()->isRunning()); //wait

        std::cout << "Run Started" << std::endl <<std::flush;
        Racer * r = this->arduino->getRacerInfo();
        std::cout << "Bib #: " << r->getBib() << std::endl  << "Start ms: " << r->getStartTime()  << std::endl << "Current Time: " << r->getStartDelay() << std::endl<< std::flush;
        std::thread * thra = new std::thread(&Arduino::receiveIntervalSignal, this->arduino);

        //this is the ui timer
        QElapsedTimer timer;
        timer.start();
        int last = timer.elapsed();
        bool done = false;

        while(!done){
            int now = timer.elapsed();

            //check if we have already started the thread to updat the current time
            if(!uiCounter){
               this->setCurrentBib(this->getLastRacer()->getBib());
                uiCounter = true;
                 th  = new std::thread([this]{
                    while(uiCounter){
                        this->currentTime =  this->getLastRacer()->getTime();
                    }
                 });
            }

            //we need to allow a small amount of time for the PC to update otherwise it glitches
            if(now - last > 5){
                 emit this->racerOnCourse(this->getCurrentTime());
                last = timer.elapsed();
            }

            //check if we received a signal from the arduino either to add a new racer to the stack or that someone has crossed the finish
            if(this->arduino->getIntervalStatus()){

                while(this->arduino->getMonitor()->isRunning()); //wait

                p = this->manager->read();
                 int option = 0;
                memcpy(&option, (int*) p->getData(), p->getSize());
                std::cout << "option: " << option << std::endl << std::flush;

                if(option == 1){ //somone crossed the finish line
                    uiCounter = false;
                    th->join();
                   int interval = 0;
                   int racerCount = 0;
                    p = this->manager->read();
                    memcpy(&interval, (int*) p->getData(), p->getSize());
                    std::cout << "Interval: " << interval << std::endl << std::flush;
                    p = this->manager->read();
                    memcpy(&racerCount, (int*) p->getData(), p->getSize());
                    Racer * finisher = this->arduino->getTimeQueue()->dequeue();
                     this->currentTime = interval -  finisher->getStartTime();
                    emit this->racerOnCourse(this->currentTime);
                    double t = this->currentTime /1000.00;
                    std::cout << "Bib #: " << finisher->getBib() << std::endl << "Start ms: " << finisher->getStartTime() << std::endl << "Time ms: " << this->currentTime << std::endl << "Time: " << t << std::endl << std::flush;
                    std::cout << "Racer Count: " << racerCount << std::endl;
                }else if(option == 4){ //a new racer is on the track
                     std::cout << "Run Started" << std::endl <<std::flush;
                     Racer * starter = this->arduino->getRacerInfo();
                    std::cout << "Bib #: " << starter->getBib() << std::endl  << "Start ms: " << starter->getStartTime()  << std::endl << "Current Time: " << starter->getTime() << std::endl<< std::flush;
                 }
\
                this->arduino->setIntervalStatus(false);

                if(this->arduino->getTimeQueue()->size() > 0){
                    thra = new std::thread(&Arduino::receiveIntervalSignal, this->arduino);
                }else{
                    done = true;
                    emit this->free();
                }
            }
       }
    } );
}
int UIController::getCurrentBib(){
    return this->bib;
}

int UIController::getCurrentTime(){
    return this->currentTime.load();
}
void UIController::setCurrentBib(int bib){
    this->bib = bib;
}
void UIController::startSerialLine(){
   this->serialLine =  this->arduino->startSerialLine();
    std::cout << "Started Monitor" << std::endl << std::flush;
}

void UIController::stopSerialLine(){
     this->arduino->stopSerialLine();
     this->serialLine->join();
     delete this->serialLine;
    std::cout << "Stopped Monitor" << std::endl << std::flush;
}

int UIController::getFinishDistance(){
    return this->arduino->getFinishDistance();
}

void UIController::setFinishDistance(int distance){
    emit busy(QString("Setting finish distance"));
    this->arduino->setFinishDistance(distance);
    emit newFinishDistance(distance);
}

void UIController::startDistanceMeasurement(){
    this->stopSerialLine();
    this->arduino->startDistanceMeasurement();
    emit measurementStarted();
}

void UIController::updateDistance(int distance){
    this->arduino->updateDistance(distance);
    emit distanceChanged(distance);
}

int UIController::getDistance(){
    return this->arduino->getDistance();
}

void UIController::setDistance(int distance){
    this->arduino->setDistance(distance);
}

void UIController::setQML(QObject *qml){
    this->qml = qml;
}

void UIController::updateInfo(){
    this->arduino->getInfo();
    emit this->infoChanged();
}
