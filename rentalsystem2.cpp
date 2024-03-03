#include <iostream>
#include <vector>
#include <ctime>
#include <bsoncxx/builder/stream/document.hpp>
#include <optional>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>

class Car;
class Customer;

class User {
    public:
        User(std::string name, int ID,std::string password)
            : name(name), ID(ID), password(password) {}

        virtual void rentCar(const Car& car) =0;
        virtual void returnCar(Car& car) =0;
        virtual void showInfo() const =0;

        virtual int getID() const = 0;

    protected:
        std::string name;
        int ID;
        std::string password;
};

class Car{
    public:
        Car(int ID, std::string model, std::string condition, std::string otherDetails)
            :ID(ID), model(model), condition(condition), otherDetails(otherDetails) {}

        void rentCar(User& user) ;
        void returnCar(User& user);


        std::string getModel() const {
            return model;
        }

        std::string getCondition() const {
            return condition;
        }

        std::string getOtherDetails() const {
            return otherDetails;
        }

        time_t getDueDate() const{
            std::cout << "Due Date: " << dueDate << std::endl;
            return dueDate;
        }

        void showInfo() const {
            std::cout << "ID: "<< ID << "Model: " << model << ", Condition: " << condition << ", Other Details: " << otherDetails << std::endl;
        }

        bool isRented() const {
            return rented;
        }

        void setID(const int newID) {
             ID = newID;
        }

        void setModel(const std::string& newModel) {
             model = newModel;
        }

        void setCondition(const std::string& newCondition) {
             condition = newCondition;
        }

        void setOtherDetails(const std::string& newOtherDetails) {
             otherDetails = newOtherDetails;
        }


        int getID() const ;

    private:
        int ID;
        std::string model;
        std::string condition;
        std::string otherDetails;
        bool rented;
        time_t dueDate;

};

void Car::rentCar(User& user) {
    rented = true;
    dueDate =time(0) + 7*24*60*60;
    user.rentCar(*this);
    std::cout<<"car rented succesfully , thank you "<<std::endl;
}

void Car::returnCar(User& user){
   // only the same customer should return this car 
   // it shoud be rented 
   if(rented==true){
    rented = false;
    user.returnCar(*this);
   }else{
    std::cout << "No cars to return"<< std::endl;
   }
}

class Customer : public User{
    public:
        Customer(std::string name, int ID, std::string password, int rentedCars = 0, int fineDue = 0, int customerRecord =50)
            :User(name, ID, password), rentedCars(rentedCars), fineDue(fineDue), customerRecord(customerRecord) {}

        void rentCar(const Car& car) override;
        void returnCar(Car& car) override;
        void showInfo() const override;

        void clearDue();
        int getID() const override;

        std::string getName() const {
            return name;
        }

        std::string getPassword() const {
            return password;
        }

        int getFineDue() const {
            return fineDue;
        }

        int getCustomerRecord() const {
            std::cout << "Customer Record: " << customerRecord << std::endl;
            return customerRecord;
        }

        void getRentedCarsID() const {
            for (const auto& rentedCarID : rentedCarIDs) {
                std::cout << rentedCarID << " ";
            }
        }
        int getRentedCars() const {
            std::cout << "Rented Cars: " << rentedCars << std::endl;
            return rentedCars;
        }

        void setID(const int newID) {
             ID = newID;
        }

        void setName(const std::string newName) {
             name = newName;
        }

        void setPassword(const std::string newPassword) {
             password = newPassword;
        }

        void setRentedCars(const int newRentedCars) {
             rentedCars = newRentedCars;
        }

        void setFineDue(const int newFineDue) {
             fineDue = newFineDue;
        }

        void setCustomerRecord(const int newCustomerRecord) {
             customerRecord = newCustomerRecord;
        }

        const std::vector<int>& getRentedCarIDs() const {
            return rentedCarIDs;
        }

    private:
        int rentedCars;
        int fineDue;
        int customerRecord;
        std::vector<int> rentedCarIDs;
};

int Customer::getID() const {
    return ID;
}

void Customer::rentCar(const Car& car)  {
    if(car.isRented()){
        if (rentedCars <3 && customerRecord >= 30){
            rentedCars++;
            rentedCarIDs.push_back(car.getID());
            std::cout << "Car rented successfully" << rentedCars << std::endl;
        }
        else{
            std::cout << "Cannot rent more cars or insufficient customerRecord" << std::endl;
        }
    }else{
        std::cout << "Car not available" << std::endl;
    }
    
}

void Customer::returnCar(Car& car) {
    rentedCars--;
    if (time(0) > car.getDueDate()) {
        fineDue += (time(0) - car.getDueDate()) / (24 * 60 * 60) * 10;  // Fine of 10 rupees per day
    }

    // Remove the returned car ID from the vector
    auto it = std::find(rentedCarIDs.begin(), rentedCarIDs.end(), car.getID());
    if (it != rentedCarIDs.end()) {
        rentedCarIDs.erase(it);
    }
}

void Customer::showInfo() const {
    std::cout << "Customer Name: " << name << ", ID: " << ID << ", Customer Record: " << customerRecord << std::endl;
    for (const auto& rentedCarID : rentedCarIDs) {
        std::cout << rentedCarID << " ";
    }
    std::cout << std::endl;
}

void Customer::clearDue() {
    fineDue = 0;
}

class Employee : public User {
public:
    Employee(std::string name, int ID, std::string password,int rentedCars = 0, int fineDue = 0, int employeeRecord = 70)
        : User(name, ID, password),rentedCars(rentedCars), fineDue(fineDue), employeeRecord(employeeRecord) {}

    void rentCar(const Car& car) override;
    void returnCar(Car& car) override;
    void showInfo() const override;

    void clearDue();
    int getID() const override;

    std::string getName() const {
        return name;
    }

    std::string getPassword() const {
        return password;
    }

    int getFineDue() const {
        return fineDue;
    }

    int getEmployeeRecord() const {
        std::cout << "Employee Record: " << employeeRecord << std::endl;
        return employeeRecord;
    }

    int getRentedCars() const {
        std::cout << "Rented Cars: " << rentedCars << std::endl;
        return rentedCars;
    }

        void setID(const int newID) {
             ID = newID;
        }

        void setName(const std::string newName) {
             name = newName;
        }

        void setPassword(const std::string newPassword) {
             password = newPassword;
        }

        void setRentedCars(const int newRentedCars) {
             rentedCars = newRentedCars;
        }

        void setFineDue(const int newFineDue) {
             fineDue = newFineDue;
        }

        void setEmployeeRecord(const int newEmployeeRecord) {
             employeeRecord = newEmployeeRecord;
        }

private:
    int rentedCars;
    int fineDue;
    int employeeRecord;

};

void Employee::rentCar(const Car& car) {
    if(car.isRented()){
        if (rentedCars < 2 && employeeRecord >= 40) {
        rentedCars++;
        std::cout << "Car rented successfully with a 15% discount!" << std::endl;
    } else {
        std::cout << "Cannot rent more cars or insufficient employee record." << std::endl;
    }
    }else{
        std::cout << "Car not Available" << std::endl;
    }
    
}

void Employee::returnCar(Car& car) {
    rentedCars--;
    if (time(0) > car.getDueDate()) {
        fineDue += (time(0) - car.getDueDate()) / (24 * 60 * 60) * 10;  //fine of 10 ruppes per day
    }
}

void Employee::showInfo() const {
    std::cout << "Employee Name: " << name << ", ID: " << ID << ", Employee Record: " << employeeRecord << std::endl;
}

void Employee::clearDue() {
    fineDue = 0;
}

class Manager : public User {
public:
    Manager(std::string name, int ID, std::string password)
        : User(name, ID, password) {}

    void showInfo() const override;
    int getID() const override;

    // void addCustomerInDatabase(mongocxx::database& database, const Customer& customer) {
    //     database.addCustomer(customer);
    // }
    // void updateCustomerInDatabase(mongocxx::database& database, const Customer& customer){
    //     database.updateCustomer(customer);
    // }
    // void deleteCustomerInDatabase(mongocxx::database& database, const Customer& customer){
    //     database.deleteCustomer(customer);
    // }

    // void addCarInDatabase(mongocxx::database& database, const Car& car){
    //     database.addCar(car);
    // }
    // void updateCarInDatabase(mongocxx::database& database, const Car& car){
    //     database.updateCar(car);
    // }
    // void deleteCarInDatabase(mongocxx::database& database, const Car& car){
    //     database.deleteCar(car);
    // }

    // void addEmployeeInDatabase(mongocxx::database& database, const Employee& employee){
    //     database.addEmployee(employee);
    // }
    // void updateEmployeeInDatabase(mongocxx::database& database, const Employee& employee){
    //     database.updateEmployee(employee);
    // }
    // void deleteEmployeeInDatabase(mongocxx::database& database, const Employee& employee){
    //     database.deleteEmployee(employee);
    // }


};



void Manager::showInfo() const {
    std::cout << "Manager Name: " << name << ", ID: " << ID << std::endl;
}



int Employee::getID() const {
    return ID;
}

int Manager::getID() const {
    return ID;
}

int Car::getID() const {
    return ID;
}

class Database {
public:
    Database(const std::string& connectionString);

    void addCar(const Car& car);
    void updateCar(const Car& car);
    void deleteCar(const Car& car);
    bool searchCar(const Car& car);

    void addCustomer(const Customer& customer);
    void updateCustomer(const Customer& customer);
    void deleteCustomer(const Customer& customer);
    bool searchCustomer(const Customer& customer);

    void addEmployee(const Employee& employee);
    void updateEmployee(const Employee& employee);
    void deleteEmployee(const Employee& employee);
    bool searchEmployee(const Employee& employee);

    void showAllCars() const;
    void showAllCustomers() const;
    void showAllEmployees() const;

    bool verifyCustomer(int customerId, const std::string& password) const;
    bool verifyEmployee(int employeeId, const std::string& password) const;
    bool verifyManager(int managerId, const std::string& password) const;

    // std::vector<Customer> getAllCustomers();

    void clearCollection(const std::string& collectionName);


private:
    mongocxx::instance instance;  // MongoDB C++ driver instance
    mongocxx::client client;      // MongoDB client
    mongocxx::database db;        // MongoDB database


    // Helper functions
    bsoncxx::document::value createCarDocument(const Car& car) const;
    bsoncxx::document::value createCustomerDocument(const Customer& customer) const;
    bsoncxx::document::value createEmployeeDocument(const Employee& employee) const;
};

// std::vector<Customer> Database::getAllCustomers() {
//     std::vector<Customer> customers;

//     auto collection = db["customers"];
//     auto cursor = collection.find({});

//     for (const auto& doc : cursor) {
//         bsoncxx::document::view customerDocument = doc;

//         if (customerDocument["name"] && customerDocument["id"] && customerDocument["password"] &&
//             customerDocument["rentedCars"] && customerDocument["fineDue"] && customerDocument["customerRecord"]) {

//             std::string name = customerDocument["name"].get_utf8().value.to_string();
//             int ID = customerDocument["id"].get_int32();
//             std::string password = customerDocument["password"].get_utf8().value.to_string();
//             int rentedCars = customerDocument["rentedCars"].get_int32();
//             int fineDue = customerDocument["fineDue"].get_int32();
//             int customerRecord = customerDocument["customerRecord"].get_int32();

//             customers.emplace_back(name, ID, password, rentedCars, fineDue, customerRecord);
//         } else {
//             std::cerr << "Error: Missing or incorrect fields in customer document." << std::endl;
//         }
//     }

//     return customers;
// }



Database::Database(const std::string& connectionString)
    : client(mongocxx::uri(connectionString)), db(client["carRentalDB"]) {
    // Initialize MongoDB
    if (!client) {
        std::cerr << "Error: Failed to connect to the database." << std::endl;
    }
    else{
        std::cout << "Connected to the database successfully!" << std::endl;
    }
}

// void Database::clearCollection(const std::string& collectionName) {
//     auto collection = db[collectionName];
//     auto result = collection.delete_many({});
//     std::cout << "Deleted " << result->deleted_count() << " documents from the collection '" << collectionName << "'." << std::endl;
// }

// Helper function to convert Car object to MongoDB document
bsoncxx::document::value Database::createCarDocument(const Car& car) const {
    using bsoncxx::builder::stream::document;
    using bsoncxx::builder::stream::open_document;
    using bsoncxx::builder::stream::close_document;
    using bsoncxx::builder::stream::finalize;

    return document{}
        << "model" << car.getModel()
        << "id" << car.getID()
        << "condition" << car.getCondition()
        << "otherDetails" << car.getOtherDetails()
        << "rented" << car.isRented()
        << "dueDate" << bsoncxx::types::b_date{std::chrono::system_clock::from_time_t(car.getDueDate())}
        << bsoncxx::builder::stream::finalize;
}

// Helper function to convert Customer object to MongoDB document
bsoncxx::document::value Database::createCustomerDocument(const Customer& customer) const {
    using bsoncxx::builder::stream::document;
    using bsoncxx::builder::stream::open_document;
    using bsoncxx::builder::stream::close_document;
    using bsoncxx::builder::stream::finalize;

    return document{}
        << "name" << customer.getName()
        << "id" << customer.getID()
        << "password" << customer.getPassword()
        << "rentedCars" << customer.getRentedCars()
        << "fineDue" << customer.getFineDue()
        << "customerRecord" << customer.getCustomerRecord()
        << bsoncxx::builder::stream::finalize;
}

// Helper function to convert Employee object to MongoDB document
bsoncxx::document::value Database::createEmployeeDocument(const Employee& employee) const {
    using bsoncxx::builder::stream::document;
    using bsoncxx::builder::stream::open_document;
    using bsoncxx::builder::stream::close_document;
    using bsoncxx::builder::stream::finalize;

    return document{}
        << "name" << employee.getName()
        << "id" << employee.getID()
        << "password" << employee.getPassword()
        << "rentedCars" << employee.getRentedCars()
        << "fineDue" << employee.getFineDue()
        << "employeeRecord" << employee.getEmployeeRecord()
        << bsoncxx::builder::stream::finalize;
}

// Add a car to the database
void Database::addCar(const Car& car) {
    auto collection = db["cars"];
    auto document = createCarDocument(car);
    collection.insert_one(document.view());
}

// Update a car in the database
void Database::updateCar(const Car& car) {
    auto collection = db["cars"];
    auto filter = bsoncxx::builder::stream::document{} << "id" << car.getID() << bsoncxx::builder::stream::finalize;
    auto update = createCarDocument(car);
    collection.update_one(filter.view(), update.view());
}

// Delete a car from the database
void Database::deleteCar(const Car& car) {
    auto collection = db["cars"];
    auto filter = bsoncxx::builder::stream::document{} << "id" << car.getID() << bsoncxx::builder::stream::finalize;
    collection.delete_one(filter.view());
}

// Search a car from the database
bool Database::searchCar(const Car& car) {
    auto collection = db["cars"];
    auto filter = bsoncxx::builder::stream::document{} << "id" << car.getID() << bsoncxx::builder::stream::finalize;

    auto result = collection.find_one(filter.view());
    if (result) {
        bsoncxx::document::view document = result->view();
        return true;
    } else {
        std::cout << "couldn't find this car in the database" << std::endl;
        return false;
    }
}

// Add a customer to the database
void Database::addCustomer(const Customer& customer) {
    auto collection = db["customers"];
    auto document = createCustomerDocument(customer);
    collection.insert_one(document.view());
}

// Update a customer in the database
void Database::updateCustomer(const Customer& customer) {
    auto collection = db["customers"];
    auto filter = bsoncxx::builder::stream::document{} << "id" << customer.getID() << bsoncxx::builder::stream::finalize;
    auto update = createCustomerDocument(customer);
    collection.update_one(filter.view(), update.view());
}

// Delete a customer from the database
void Database::deleteCustomer(const Customer& customer) {
    auto collection = db["customers"];
    auto filter = bsoncxx::builder::stream::document{} << "id" << customer.getID() << bsoncxx::builder::stream::finalize;
    collection.delete_one(filter.view());
}

// Search a customer in the database
bool Database::searchCustomer(const Customer& customer) {
    auto collection = db["customers"];
    auto filter = bsoncxx::builder::stream::document{} << "ID" << customer.getID() << bsoncxx::builder::stream::finalize;

    auto result = collection.find_one(filter.view());
    if (result) {
        bsoncxx::document::view document = result->view();
        return true;
    } else {
        std::cout << "Couldn't find the Customer in the database " << std::endl;
        return false;
    }
}

// Add an employee to the database
void Database::addEmployee(const Employee& employee) {
    auto collection = db["employees"];
    auto document = createEmployeeDocument(employee);
    collection.insert_one(document.view());
}

// Update an employee in the database
void Database::updateEmployee(const Employee& employee) {
    auto collection = db["employees"];
    auto filter = bsoncxx::builder::stream::document{} << "id" << employee.getID() << bsoncxx::builder::stream::finalize;
    auto update = createEmployeeDocument(employee);
    collection.update_one(filter.view(), update.view());
}

// Delete an employee from the database
void Database::deleteEmployee(const Employee& employee) {
    auto collection = db["employees"];
    auto filter = bsoncxx::builder::stream::document{} << "id" << employee.getID() << bsoncxx::builder::stream::finalize;
    collection.delete_one(filter.view());
}

// search for a employee in the database
bool Database::searchEmployee(const Employee& employee) {
    auto collection = db["employees"];
    auto filter = bsoncxx::builder::stream::document{} << "id" << employee.getID() << bsoncxx::builder::stream::finalize;

    auto result = collection.find_one(filter.view());
    if (result) {
        bsoncxx::document::view document = result->view();
        return true;
    } else {
        std::cout << "Couldn't find the employee in the database " << std::endl;
        return false;
    }
}

// Show all cars in the database
void Database::showAllCars() const {
    auto collection = db["cars"];
    auto cursor = collection.find({});
    for (auto&& doc : cursor) {
        std::cout << bsoncxx::to_json(doc) << std::endl;
    }
}

// Show all customers in the database
void Database::showAllCustomers() const {
    auto collection = db["customers"];
    auto cursor = collection.find({});
    for (auto&& doc : cursor) {
        std::cout << bsoncxx::to_json(doc) << std::endl;
    }
}

// Show all employees in the database
void Database::showAllEmployees() const {
    auto collection = db["employees"];
    auto cursor = collection.find({});
    for (auto&& doc : cursor) {
        std::cout << bsoncxx::to_json(doc) << std::endl;
    }
}

bool Database::verifyCustomer(int customerId, const std::string& password) const {
    auto collection = db["customers"];
    auto filter = bsoncxx::builder::stream::document{} << "id" << customerId << "password" << password << bsoncxx::builder::stream::finalize;

    auto result = collection.find_one(filter.view());
    return result ? true : false;
}

bool Database::verifyEmployee(int employeeId, const std::string& password) const {
    auto collection = db["employees"];
    auto filter = bsoncxx::builder::stream::document{} << "id" << employeeId << "password" << password << bsoncxx::builder::stream::finalize;

    auto result = collection.find_one(filter.view());
    return result ? true : false;
}

bool Database::verifyManager(int managerId, const std::string& password) const {
    const int knownManagerId = 1;
    const std::string knownManagerPassword = "manager123";

    return (managerId == knownManagerId) && (password == knownManagerPassword);
}

int main() {
    // Sample usage of the car rental system
    

    std::string atlasConnectionString = "mongodb+srv://swecha:amruta@cluster0.4o1cvll.mongodb.net/<dbname>?retryWrites=true&w=majority";
    Database database(atlasConnectionString);

    Car car1(1,"Toyota", "Good", "Red");
    Car car2(2,"Honda", "Excellent", "Blue");
    Car car3(3,"Ford", "Average", "Silver");
    Car car4(4,"Chevrolet", "Fair", "Black");
    Car car5(5,"Nissan", "Excellent", "White");


    Customer customer1("John Doe", 1, "pass123",0, 10, 20);
    Customer customer2("Bob Johnson", 2, "pass789", 0, 5, 40);
    Customer customer3("Eva Williams", 3, "pass101", 3, 15, 80);
    Customer customer4("Michael Davis", 4, "pass202", 2, 8, 55);
    Customer customer5("Sophia Brown", 5, "pass303", 1, 5, 65);

    Employee employee1("Alice", 101, "empPass", 2, 20, 70);
    Employee employee2("Bob", 102, "empPass123", 1, 15, 80);
    Employee employee3("Charlie", 103, "empPass456", 3, 25, 90);
    Employee employee4("Eva Davis", 104, "passabc", 3, 20, 30);
    Employee employee5("David", 105, "empPass789", 0, 0, 60);

    int newID;
    
    std::vector<Customer> customers;
    
    int customerId;
    std::string customerPassword;

    int updateCustomerID;
    Customer customerToUpdate(" ",0," ",0,0,0);

    std::string newName,newPassword;
    int newRentedCars, newFineDue, newCustomerRecord;


    std::vector<Employee> employees;

    std::vector<Car> cars;

    int carID;

    std::string carModel;
    int updateCarID;

    std::string newModel, newCondition, newOtherDetails;
    Car carToUpdate(0, " "," "," ");

    int employeeId;
    std::string employeePassword;

    int updateEmployeeID;
    Employee employeeToUpdate(" ",0," ",0,0,0);

    
    int newEmployeeRecord;

    customers.push_back(customer1);
    customers.push_back(customer2);
    customers.push_back(customer3);
    customers.push_back(customer4);
    customers.push_back(customer5);

    cars.push_back(car1);
    cars.push_back(car2);
    cars.push_back(car3);
    cars.push_back(car4);
    cars.push_back(car5);

    employees.push_back(employee1);
    employees.push_back(employee2);
    employees.push_back(employee3);
    employees.push_back(employee4);
    employees.push_back(employee5);

    
    int choice;
    do {
        //choose who is visiting 
        std::cout << "\n******** Car Rental System profile********\n";
        std::cout << "1. Customer\n";
        std::cout << "2. Employee\n";
        std::cout << "3. Manager\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1: 
                  // Verify Customer ID and Password
                  std::cout << "Enter Customer ID: ";
                  std::cin >> customerId;
                  std::cout << "Enter Customer Password: ";
                  std::cin >> customerPassword;

                  // Customer verification logic
                  if (database.verifyCustomer(customerId, customerPassword)) {
                    std::cout << "Customer Login Successful!\n";
                    int customerChoice;
                    std::cout << "\n******** Car Rental System Customer Menu********\n";
                    std::cout << "1. Show profile\n";
                    std::cout << "2. Rent Car\n";
                    std::cout << "3. Return Car\n";
                    std::cout << "4. Rented Cars\n";
                    std::cout << "5. Customer Record\n";
                    std::cout << "0. Exit\n";
                    std::cout << "Enter your choice: ";
                    std::cin >> customerChoice;

                    switch(customerChoice) {
                        case 1:
                            for (const auto& customer : customers) {
                                if (customer.getID() == customerId) {
                                    customer.showInfo();
                                    break;  // Assuming customer IDs are unique, exit loop once found
                                }
                            }
                            break;

                        case 2:
                            database.showAllCars() ;
                            
                            std::cout << "select your car id: ";
                            std::cin >> carID;
                            for ( auto& car : cars) {
                               if (car.getID() == carID) {
                                 for( auto& customer : customers) {
                                    if(customer.getID() == customerId)
                                       car.rentCar(customer);
                                       std::cout << "car rented successfully";
                                   break;  // Assuming customer IDs are unique, exit loop once found
                                }
                             }
                            }
                            break;
                            // 
                            //  for (const auto& customer : customers) {
                            //     if (customer.getID() == customerId) {
                                    
                            //         for(const auto& car:cars){
                            //               if(car.getID()==carID){
                            //                   customer.rentCar(car);
                            //                 }
                            //         }
                            //          break;  // Assuming customer IDs are unique, exit loop once found
                            //      }
                            //  }
                            break;

                        case 3:
                            for( auto& customer : customers) {
                                    if(customer.getID() == customerId){
                                        customer.getRentedCarsID();
                                    }
                            }
                            std::cout << "select your car id that you wanted to return: ";
                            std::cin >> carID;
                            for ( auto& car : cars) {
                               if (car.getID() == carID) {
                                 for( auto& customer : customers) {
                                    if(customer.getID() == customerId){
                                       car.returnCar(customer);
                                    }
                                       
                                   break;  // Assuming customer IDs are unique, exit loop once found
                                }
                             }
                            }
                            // for (const auto& customer : customers) {
                            //     if (customer.getID() == customerId) {
                            //         customer.returnCar();
                            //         break;  // Assuming customer IDs are unique, exit loop once found
                            //     }
                            // }
                            break;

                        case 4:
                            for (const auto& customer : customers) {
                                if (customer.getID() == customerId) {
                                    customer.getRentedCarsID();
                                    break;  // Assuming employees IDs are unique, exit loop once found
                                }
                            }
                            
                            break;

                        case 5:
                            for (const auto& customer : customers) {
                                if (customer.getID() == customerId) {
                                    customer.getCustomerRecord();
                                    break;  // Assuming customer IDs are unique, exit loop once found
                                }
                            }
                            break;
                    
                        case 0:
                            // Exit the program
                            break;

                        default:
                           std::cout << "Invalid choice. Please enter a valid option.\n"; 
                           break;
                    }
                    
                  } else {
                    std::cout << "Invalid Customer ID or Password. Login failed.\n";
                  }

                 break;
                  
                  
            
            case 2:
                  

                  // Verify Employee ID and Password
                  std::cout << "Enter Employee ID: ";
                  std::cin >> employeeId;
                  std::cout << "Enter Employee Password: ";
                  std::cin >> employeePassword;

                  // Employee verification logic
                  if (database.verifyEmployee(employeeId, employeePassword)) {
                    std::cout << "Employee Login Successful!\n";
                    int employeeChoice;
                    std::cout << "\n******** Car Rental System Employee Menu********\n";
                    std::cout << "1. Show profile\n";
                    std::cout << "2. Rent Car\n";
                    std::cout << "3. Return Car\n";
                    std::cout << "4. Rented Cars\n";
                    std::cout << "5. Employee Record\n";
                    std::cout << "0. Exit\n";
                    std::cout << "Enter your choice: ";
                    std::cin >> employeeChoice;

                    switch(employeeChoice) {
                        case 1:
                            for (const auto& employee : employees) {
                                if (employee.getID() == employeeId) {
                                    employee.showInfo();
                                    break;  // Assuming employees IDs are unique, exit loop once found
                                }
                            }break;

                        case 2:
                            database.showAllCars() ;
                            
                            std::cout << "select your car id: ";
                            std::cin >> carID;
                            for ( auto& car : cars) {
                               if (car.getID() == carID) {
                                 for( auto& employee : employees) {
                                    if(employee.getID() == employeeId)
                                       car.rentCar(employee);
                                       std::cout << "car rented successfully";
                                   break;  // Assuming customer IDs are unique, exit loop once found
                                }
                             }
                            }
                            break;
                            // for (const auto& employee : employees) {
                            //     if (employee.getID() == employeeId) {
                            //         employee.rentCar();
                            //         break;  // Assuming employees IDs are unique, exit loop once found
                            //     }
                            // }
                            
                            break;

                        case 3:
                            // for (const auto& employee : employees) {
                            //     if (employee.getID() == employeeId) {
                            //         employee.returnCar();
                            //         break;  // Assuming employees IDs are unique, exit loop once found
                            //     }
                            // }
                            std::cout << "select your car id that you wanted to return: ";
                            std::cin >> carID;
                            for ( auto& car : cars) {
                               if (car.getID() == carID) {
                                 for( auto& employee : employees) {
                                    if(employee.getID() == employeeId){
                                       car.returnCar(employee);
                                    }
                                       
                                   break;  // Assuming customer IDs are unique, exit loop once found
                                }
                             }
                            }
                            break;

                        case 4:
                            for (const auto& employee : employees) {
                                if (employee.getID() == employeeId) {
                                    employee.getRentedCars();
                                    break;  // Assuming employees IDs are unique, exit loop once found
                                }
                            }
                            break;

                        case 5:
                            for (const auto& employee : employees) {
                                if (employee.getID() == employeeId) {
                                    employee.getEmployeeRecord();
                                    break;  // Assuming employees IDs are unique, exit loop once found
                                }
                            }break;

                        case 0:
                          // Exit the program
                        break;

                        default:
                           std::cout << "Invalid choice. Please enter a valid option.\n";
                           break;
                    }  
                    
                  } else {
                    std::cout << "Invalid Employee ID or Password. Login failed.\n";
                  }

                  break;
                    

            case 3:
                int managerId;
                std::string managerPassword;

                // Verify Manager ID and Password
                std::cout << "Enter Manager ID: ";
                std::cin >> managerId;
                std::cout << "Enter Manager Password: ";
                std::cin >> managerPassword;

                // Manager verification logic
                if (database.verifyManager(managerId, managerPassword)) {
                  std::cout << "Manager Login Successful!\n";

                    // Manager operations
                  int managerChoice;
                  std::cout << "\n******** Car Rental System manager Menu********\n";
                  std::cout << "1. show all cars\n";
                  std::cout << "2. show all customers\n";
                  std::cout << "3. show all employees\n";
                  std::cout << "4. database operations for cars\n";
                  std::cout << "5. database operations for customers\n";
                  std::cout << "6. database operations for employee\n";
                  std::cout << "0. Exit\n";
                  std::cout << "Enter your choice: ";
                  std::cin >> managerChoice;

                  switch(managerChoice) {
                    case 1:
                        database.showAllCars();
                        break;

                    case 2:
                        database.showAllCustomers();
                        break;

                    case 3:
                        database.showAllEmployees();
                        break;

                    case 4:
                        int carOperation;
                           std::cout << "\n******** Car Rental System manager operations********\n";
                           std::cout << "1. update car\n";
                           std::cout << "2. add a car\n";
                           std::cout << "3. delete a car\n";
                           std::cout << "0. Exit\n";
                           std::cout << "Enter your choice: ";
                           std::cin >> carOperation;

                           switch(carOperation){
                            case 1:
                                  
                                  std::cout << "Enter the ID of the car to update: ";
                                  std::cin >> updateCarID ;

                                  

                                  carToUpdate.setID(updateCarID);

                                  if (database.searchCar(carToUpdate)) {
                                   // Car found, proceed with update
                                     

                                     std::cout << "Enter the new model: ";
                                     std::cin >> newModel;

                                     std::cout << "Enter the new condition: ";
                                     std::cin >> newCondition;

                                     std::cout << "Enter the new other details: ";
                                     std::cin >> newOtherDetails;

                                     carToUpdate.setModel(newModel);
                                     carToUpdate.setCondition(newCondition);
                                     carToUpdate.setOtherDetails(newOtherDetails);

                                     database.updateCar(carToUpdate);
                                     std::cout << "Car details updated successfully!\n";
                                  } else {
                                     std::cout << "Car not found. Update failed.\n";
                                  }
                                  break;
                               
                                
                            case 2:
                                 {
                                     std::cout << "Enter the ID of the new car: ";
                                     std::cin >> newID;

                                     std::cout << "Enter the model of the new car: ";
                                     std::cin >> newModel;

                                     std::cout << "Enter the condition of the new car: ";
                                     std::cin >> newCondition;

                                     std::cout << "Enter the otherDetails of the new car: ";
                                     std::cin >> newOtherDetails;

                                     carToUpdate.setID(newID);
                                     carToUpdate.setModel(newModel);
                                     carToUpdate.setCondition(newCondition);
                                     carToUpdate.setOtherDetails(newOtherDetails);

                                     database.addCar(carToUpdate);
                                     std::cout << "New car added to the database!\n";
                                 }
                                 break;

                            case 3:
                                 {
                                     std::cout << "Enter the ID of the car to delete: ";
                                     std::cin >> updateCarID;


                                     for (const auto& car : cars) {
                                          if (car.getID() == updateCarID) {
                                            //  newPassword=customer.getPassword();
                                            database.deleteCar(car);
                                            std::cout << "Car deleted from the database!\n";

                                          }
                                          
                                     }
                                 }
                                 break;

                            case 0:
                                 // Exit the program
                                break;

                            default:
                                std::cout << "Invalid choice. Please enter a valid option.\n";
                                break;
                           }
                        break;

                     case 5:
                        int customerOperation;
                           std::cout << "\n******** Car Rental System manager operations********\n";
                           std::cout << "1. update customer\n";
                           std::cout << "2. add a customer\n";
                           std::cout << "3. delete a customer\n";
                           std::cout << "0. Exit\n";
                           std::cout << "Enter your choice: ";
                           std::cin >> customerOperation;

                           switch(customerOperation){
                            case 1:
                                  
                                  std::cout << "Enter the ID of the customer to update: ";
                                  std::cin >> updateCustomerID;

                                  customerToUpdate.setID(updateCustomerID);

                                  if (database.searchCustomer(customerToUpdate)) {
                                       // Customer found, proceed with update
                                      std::string newName,newPassword;
                                      int newRentedCars, newCustomerFine, updateCustomerRecord;

                                      std::cout << "Enter the new name: ";
                                      std::cin >> newName;
                                      for (const auto& customer : customers) {
                                          if (customer.getID() == updateCustomerID) {
                                             newPassword=customer.getPassword();
                                          }
                                      }
                                      std::cout << "Enter the new number of rented cars: ";
                                      std::cin >> newRentedCars;
                                      std::cout << "Enter the new value of fine_due: ";
                                      std::cin >> newFineDue;
                                      std::cout << "Enter the new value of customer record value: ";
                                      std::cin >> updateCustomerRecord;

                                      customerToUpdate.setName(newName);
                                      customerToUpdate.setPassword(newPassword);
                                      customerToUpdate.setRentedCars(newRentedCars);
                                      customerToUpdate.setFineDue(newFineDue);
                                      customerToUpdate.setCustomerRecord(newCustomerRecord);

                                      database.updateCustomer(customerToUpdate);
                                      std::cout << "Customer details updated successfully!\n";
                                  } else {
                                      std::cout << "Customer not found. Update failed.\n";
                                  }
                                  break;
                                
                            case 2:
                                  {

                                     std::cout << "Enter the name of the new customer: ";
                                     std::cin.ignore(); // Ignore the newline character left in the buffer
                                     std::getline(std::cin, newName);


                                     std::cout << "Enter the id of the new customer: ";
                                     std::cin >> newID;

                                     std::cout << "Enter the password of the new customer: ";
                                     std::cin >> newPassword;

                                     std::cout << "Enter the number of rented cars for the new customer: ";
                                     std::cin >> newRentedCars;

                                     std::cout << "Enter the fine due for the new customer: ";
                                     std::cin >> newFineDue;

                                     std::cout << "Enter the customer record for the new customer: ";
                                     std::cin >> newCustomerRecord;

                                      customerToUpdate.setID(newID);
                                      customerToUpdate.setName(newName);
                                      customerToUpdate.setPassword(newPassword);
                                      customerToUpdate.setRentedCars(newRentedCars);
                                      customerToUpdate.setFineDue(newFineDue);
                                      customerToUpdate.setCustomerRecord(newCustomerRecord);

                                     database.addCustomer(customerToUpdate);
                                     std::cout << "New customer added to the database!\n";
                                  }
                                  break;

                            case 3:
                                  {
                                     std::cout << "Enter the ID of the customer to delete: ";
                                     std::cin >> updateCustomerID;

                                     for (const auto& customer : customers) {
                                          if (customer.getID() == updateCustomerID) {
                                            //  newPassword=customer.getPassword();
                                            database.deleteCustomer(customer);
                                            std::cout << "Customer deleted from the database!\n";
                                          }
                                          
                                     }

                                  }
                                  break;

                            case 0:
                                 // Exit the program
                                break;

                            default:
                                std::cout << "Invalid choice. Please enter a valid option.\n";
                                break;
                           }
                        break;
                     case 6:
                              int employeeOperation;
                           std::cout << "\n******** Car Rental System manager operations********\n";
                           std::cout << "1. update employee\n";
                           std::cout << "2. add a employee\n";
                           std::cout << "3. delete a employee\n";
                           std::cout << "0. Exit\n";
                           std::cout << "Enter your choice: ";
                           std::cin >> employeeOperation;

                              switch(employeeOperation){
                                case 1:
                                
                                std::cout << "Enter the ID of the employee to update: ";
                                std::cin >> updateEmployeeID;

                                employeeToUpdate.setID(updateEmployeeID);

                                if (database.searchEmployee(employeeToUpdate)) {
                                     
                                     std::cout << "Enter the new name: ";
                                     std::cin >> newName;
                                      for (const auto& employee : employees) {
                                          if (employeeToUpdate.getID() == updateEmployeeID) {
                                             newPassword=employeeToUpdate.getPassword();
                                          }
                                      }

                                     std::cout << "Enter the new number of rented cars: ";
                                     std::cin >> newRentedCars;

                                     std::cout << "Enter the new fine due: ";
                                     std::cin >> newFineDue;

                                     std::cout << "Enter the new employee record: ";
                                     std::cin >> newEmployeeRecord;

                                      employeeToUpdate.setName(newName);
                                      employeeToUpdate.setPassword(newPassword);
                                      employeeToUpdate.setRentedCars(newRentedCars);
                                      employeeToUpdate.setFineDue(newFineDue);
                                      employeeToUpdate.setEmployeeRecord(newEmployeeRecord);
 
                                     database.updateEmployee(employeeToUpdate);
                                     std::cout << "Employee details updated successfully!\n";
                                } else {
                                     std::cout << "Employee not found. Update failed.\n";
                                }
                                
                                break;

                            case 2:
                                {

                                     std::cout << "Enter the name of the new employee: ";
                                     std::cin.ignore(); // Ignore the newline character left in the buffer
                                     std::getline(std::cin, newName);

                                     std::cout << "Enter the id of the new employee: ";
                                     std::cin >> newID;

                                     std::cout << "Enter the password of the new employee: ";
                                     std::cin >> newPassword;

                                     std::cout << "Enter the number of rented cars for the new employee: ";
                                     std::cin >> newRentedCars;

                                     std::cout << "Enter the fine due for the new employee: ";
                                     std::cin >> newFineDue;

                                     std::cout << "Enter the employee record for the new employee: ";
                                     std::cin >> newEmployeeRecord;

                                      employeeToUpdate.setName(newName);
                                      employeeToUpdate.setID(newID);
                                      employeeToUpdate.setPassword(newPassword);
                                      employeeToUpdate.setRentedCars(newRentedCars);
                                      employeeToUpdate.setFineDue(newFineDue);
                                      employeeToUpdate.setEmployeeRecord(newEmployeeRecord);

                                     database.addEmployee(employeeToUpdate);
                                     std::cout << "New employee added to the database!\n";
                                }
                                break;

                            case 3:
                                {
                                     std::cout << "Enter the ID of the employee to delete: ";
                                     std::cin >> updateEmployeeID;

                                     for (const auto& employee : employees) {
                                          if (employee.getID() == updateEmployeeID) {
                                            //  newPassword=customer.getPassword();
                                            database.deleteEmployee(employee);
                                            std::cout << "Employee deleted from the database!\n";
                                          }
                                     }
                                }
                                break;

                            case 0: 
                               break;

                            default:
                               std::cout << "Invalid choice for Database Operations.\n";
                               break;
                           }

                        break;

                    case 0:
                        //get back to main menu
                        break;

                    default:
                        std::cout << "Invalid choice for Database Operations.\n";
                        break;

                  }
                } else {
                    std::cout << "Invalid Manager ID or Password. Login failed.\n";
                }

            
        }
       

    } while (choice != 0);

    return 0;
}