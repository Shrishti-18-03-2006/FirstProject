📦 PROJECT1 — C++ MySQL Shopping Application

A console-based shopping management application written in C++ with MySQL as the backend.
The system supports user registration, secure login, customer profile creation, product browsing, cart management, and checkout.

This project is structured to mimic real-world e-commerce workflows and demonstrates database connectivity, modular C++ architecture, and full-stack logic.

🚀 Features Implemented
🔹 User System

Two user flows:

New User

Creates a login (email + password)

Enters customer details (Name, Contact, Address)

System auto-detects new Customer ID

Redirects to product & cart system

Existing User

Logs in using stored email & password

System fetches Customer ID from database

Redirects to products & cart

Password validation rules:

Minimum 8 characters

Must include uppercase, lowercase, digit & special character

Stored in uppercase LOGIN table (case-sensitive on Linux).

🔹 Customer Management

Ability to add customer details:

Name

Contact Number

Email

Address

Stored in CUSTOMER_DETAILS table.

Auto-increment ID retrieval using LAST_INSERT_ID().

🔹 Product Browsing & Selection

Products are loaded dynamically from the PRODUCT table, supporting:

List all categories

List subcategories

List products by category/subcategory

Detailed product view:

Name

Category

Subcategory

Company

Stock

Price

Expiry Date

Supplier information fetched from Supplier table based on SID.

🔹 Shopping Cart System

Each user has a unique cart linked via Customer_ID.

Cart supports:

Add to cart

Update quantity

Remove item

View cart

Checkout

Checkout:

Auto reduces stock from PRODUCT table

Clears cart after completion

🏗️ Project Architecture
📁 Folder Structure
PROJECT1/
│
├── CaptainLevi/              # All .cpp files
│   ├── FinalMain.cpp         # Main application flow
│   ├── Customer.cpp
│   ├── Product.cpp
│   ├── Supplier.cpp
│   ├── Date.cpp
│   ├── DBFunctions.cpp
│   ├── Functions.cpp
│   ├── login.cpp
│   ├── Cart.cpp
│   ├── CartFunctions.cpp
│
├── MissionPlan/              # All .h header files
│   ├── Customer.h
│   ├── Product.h
│   ├── Supplier.h
│   ├── Date.h
│   ├── DBFunctions.h
│   ├── Functions.h
│   ├── login.h
│   ├── Cart.h
│   ├── CartFunctions.h
│
└── README.md                 # This file

🛢️ Database Schema (MySQL)
1. LOGIN
Column	Type	Notes
Email	VARCHAR(255)	Primary Key
Password	VARCHAR(255)	Plain text (for now)
2. CUSTOMER_DETAILS
Column	Type	Notes
ID	INT	Auto-increment PK
Name	VARCHAR(50)	
Contact_Num	VARCHAR(20)	
Email	VARCHAR(255)	FK → LOGIN.Email
Address	VARCHAR(255)	
3. PRODUCT
Column	Type
Product_ID	INT PK
Product_Name	VARCHAR(255)
Category	VARCHAR(255)
Subcategory	VARCHAR(255)
Price	DOUBLE
Stock_Qtn	INT
Company_name	VARCHAR(255)
ExpiryDate	DATE
SID	INT FK → Supplier
4. Supplier
Column	Type
SID	INT PK
Sname	VARCHAR(255)
Contact_Num	VARCHAR(20)
Email	VARCHAR(255)
Address	VARCHAR(255)
5. Cart
Column	Type	Notes
Cart_ID	INT PK	Auto-increment
Customer_ID	INT FK	→ CUSTOMER_DETAILS.ID
Product_ID	INT FK	→ PRODUCT.Product_ID
Quantity	INT	
AddedDate	DATETIME	Defaults to CURRENT_TIMESTAMP
🧠 Key Logic Implemented
✔ New User Workflow
Enter email → Create login record → Enter customer details → Auto get CustomerID → Category Menu

✔ Existing User Workflow
Login → Authenticate → Fetch customerID → Category Menu

✔ Cart Workflow
Add product → Check stock → If exists update quantity → Checkout reduces stock → Clear cart

⚙️ Compiling & Running
Compile

Run inside the PROJECT1 folder:

g++ -o final CaptainLevi/*.cpp -I/usr/include/mysql-cppconn-8/ -lmysqlcppconn

Run
./final

💡 Future Enhancements (Planned / Optional)

Admin panel (add/update/delete products)

Order history system

Profile update system

Product search

Sort by price, stock, brand

Coupon/discount logic

Password hashing (bcrypt)

PDF invoice generation

👤 Author

Shrishti —C++ with MySQL