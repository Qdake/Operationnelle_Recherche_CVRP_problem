#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>

#define DEBUG
class Client{
    public:
    static int objectCount;
    int number;
    float demand;
    Client(float demand_):demand(demand_){
        objectCount++;
        this->number = objectCount;
    };
    ~Client(){};
    //print
    std::ostream& print(std::ostream& out)const{
        out<<this->demand;
        return out;
    };
};
int Client::objectCount{0};
std::ostream& operator<<(std::ostream& out, const Client& Client){
    Client.print(out);
    return out;
} 
class Car{
    public:
    static int objectCount;
    int number;
    float capacity;
    float demand;
    std::vector<Client> clients;
    //constructor
    Car(float capacity_):capacity(capacity_),demand(0){
        objectCount++;
        number = objectCount;
    };
    //destructor
    ~Car(){
    };
    //add_client
    bool add_client(const Client& client){
        if (client.demand + this->demand > this->capacity)
            return false;
        this->demand += client.demand;
        this->clients.push_back(client);
        return true;
    };
    //print
    std::ostream& print(std::ostream& out)const{
        out<<"car "<<this->number<<" :";
        for (const Client& client:this->clients)
            out<<client<<" ";
        return out;
    }
};
int Car::objectCount{0};
std::ostream& operator<<(std::ostream& out, const Car& car){
    car.print(out);
    return out;
}
bool comparator(const Client& client1,const Client& client2){
    return (client1.demand>client2.demand)?true:false;
}
bool first_fit_decreasing(std::vector<Car>& cars,std::vector<Client>& clients){
    std::sort(clients.begin(),clients.end(),comparator);

    #ifdef DEBUG
        for (auto i:clients)
            std::cout<<i<<" ";
        std::cout<<std::endl;
    #endif

    for (Client& client:clients){
        bool success{false};
        for (Car& car:cars){
            success = car.add_client(client);
            if (success)
                break;
        };
        if (!success)
            return false;
    };
    return true;
}

int main(){
    std::vector<float> demands{3,6,2,3,2,7};
    float capacity = 10;
    int k = 3;
    std::vector<Client> clients;
    for (auto demand:demands)
        clients.push_back(Client(demand));
    std::vector<Car> cars;
    for (int i{0};i<k;i++)
        cars.push_back(Car(capacity));
    bool success = first_fit_decreasing(cars,clients);
    std::cout<<"found a solution? "<< success<<std::endl;
    for (auto car:cars)
        std::cout<<car<<std::endl;
    std::cout<<"cars.size = "<<cars.size()<<std::endl;


    // std::vector<float> demands{8,5,7,5};
    // float capacity = 10;
    // int k = 2;
    // std::vector<Client> clients;
    // for (auto demand:demands)
    //     clients.push_back(Client(demand));
    // std::vector<Car> cars;
    // for (int i{0};i<k;i++)
    //     cars.push_back(Car(capacity));
    // bool success = first_fit_decreasing(cars,clients);
    // std::cout<<"found a solution? "<< success<<std::endl;
    // for (auto car:cars)
    //     std::cout<<car<<std::endl;
    // std::cout<<"cars.size = "<<cars.size()<<std::endl;


    return 0;
};