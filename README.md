# c-assignment

The main file to be executed is the rentalsystem2. The given code was connected to the mongoDB database.
For the file to connect to the database it is required to install the mongoDB c++ driver and also connecting the database with the connection string.

The Command used to compile the code is 

g++ rentalsystem2.cpp -o rentalsystem2 $(pkg-config --cflags --libs libmongocxx) -std=c++11. 

The code can be executed using the command ./rentalsystem2.

The Main classes in the code were User, Car, Customer, Employee, Manager, Database. Each class consists of different functions. 

After the execution of the program, the program asks to select the usertype

--customer

--employee

--manager

and asks to login them using their login Id, password.

Now after selecting the usertype and successful login. each usertype has different type of functions to select.

For example, for Customer usertype the functions they can select are 

--Show Profile

--Rent Car

--Return Car

--Rented Cars

--Customer Record.

for Employee usertype the functions they can select are 

--Show Profile

--Rent Car

--Return Car

--Rented Cars

--Employee Record.

for manager usertype the functions they can select are 

--show all cars

--show all customers

--show all Employees

--database operations on cars

--database operations on customers

--database operations on employees.
