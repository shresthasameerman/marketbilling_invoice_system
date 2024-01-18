#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm>

using namespace std;

class Product {
public:
    int pcode;
    float price;
    float discount;
    string pname;

    virtual void display() const = 0;
    virtual float calculateCost(int quantity) const = 0;
};

class Electronics : public Product {
private:
    string brand;

public:
    Electronics(int code, float pr, float disc, const string& name, const string& b)
        : brand(b) {
        pcode = code;
        price = pr;
        discount = disc;
        pname = name;
    }

    void display() const override {
        cout << "Electronics: " << pname << ", Price: $" << price << ", Brand: " << brand;
    }

    float calculateCost(int quantity) const override {
        return price * quantity * 1.1;  // Applying a 10% tax for electronics
    }
};

class Clothing : public Product {
private:
    string size;

public:
    Clothing(int code, float pr, float disc, const string& name, const string& s)
        : size(s) {
        pcode = code;
        price = pr;
        discount = disc;
        pname = name;
    }

    void display() const override {
        cout << "Clothing: " << pname << ", Price: $" << price << ", Size: " << size;
    }

    float calculateCost(int quantity) const override {
        return price * quantity * 0.9;  // Applying a 10% discount for clothing
    }
};

class Groceries : public Product {
private:
    string category;

public:
    Groceries(int code, float pr, float disc, const string& name, const string& cat)
        : category(cat) {
        pcode = code;
        price = pr;
        discount = disc;
        pname = name;
    }

    void display() const override {
        cout << "Groceries: " << pname << ", Price: $" << price << ", Category: " << category;
    }

    float calculateCost(int quantity) const override {
        return price * quantity;
    }
};

class Customer {
public:
    string name;
    int customerId;

    Customer(const string& n, int id)
        : name(n), customerId(id) {}
};

class Invoice {
private:
    int invoiceNumber;
    Customer customer;
    vector<Product*> products;

public:
    Invoice(int invNum, const Customer& c)
        : invoiceNumber(invNum), customer(c) {}

    void addProduct(Product* product) {
        products.push_back(product);
    }

    void generateInvoice() const {
        cout << "\n\nInvoice #" << invoiceNumber << "\n";
        cout << "Customer: " << customer.name << "\n";
        cout << "----------------------------------------\n";
        cout << "Products Purchased:\n";

        float totalAmount = 0;

        for (const auto& product : products) {
            product->display();
            cout << "\n";
            totalAmount += product->calculateCost(1);  // Assuming quantity is 1 for simplicity
        }

        cout << "----------------------------------------\n";
        cout << "Total Amount: $" << totalAmount << "\n";
    }
};

class ShoppingCart {
private:
    vector<Product*> availableProducts;
    vector<Customer> customers;
    vector<Invoice> invoices;
    int productCodeCounter;
    int customerIDCounter;
    int invoiceNumberCounter;

public:
    ShoppingCart()
        : productCodeCounter(1), customerIDCounter(1), invoiceNumberCounter(1) {}

    void menu();
    void administrator();
    void buyer();
    void addProduct();
    void editProduct();
    void removeProduct();
    void listProducts() const;
    void addCustomer();
    void listCustomers() const;
    void createInvoice();
    void listInvoices() const;
};

void ShoppingCart::menu() {
    while (true) {
        int choice;
        cout << "\n\n___________________________________\n";
        cout << "     Nepali Supermarket Menu       \n";
        cout << "___________________________________\n";
        cout << " ___________________________________\n";
        cout << "|                                   |\n";
        cout << "|   1) Administrator                |\n";
        cout << "|   2) Customer                     |\n";
        cout << "|   3) Exit                          |\n";
        cout << "|___________________________________|\n";
        cout << "Please select your option: ";
        cin >> choice;

        switch (choice) {
        case 1:
            administrator();
            break;

        case 2:
            buyer();
            break;

        case 3:
            return;

        default:
            cout << "Please select from the given options.\n";
        }
    }
}

void ShoppingCart::administrator() {
    while (true) {
        int choice;
        cout << "\n\n     Administrator menu\n";
        cout << "______________________________________\n";
        cout << "|=======1) Add a new Product=========|\n";
        cout << "|=======2) Edit a product info=======|\n";
        cout << "|=======3) Remove a product===========|\n";
        cout << "|=======4) List all products==========|\n";
        cout << "|=======5) Add a new Customer=========|\n";
        cout << "|=======6) List all customers=========|\n";
        cout << "|=======7) Create Invoice=============|\n";
        cout << "|=======8) List all invoices==========|\n";
        cout << "|=======9) Return to Main Menu========|\n";
        cout << "|____________________________________|\n";

        cout << "Please select your option: ";
        cin >> choice;

        switch (choice) {
        case 1:
            addProduct();
            break;

        case 2:
            editProduct();
            break;

        case 3:
            removeProduct();
            break;

        case 4:
            listProducts();
            break;

        case 5:
            addCustomer();
            break;

        case 6:
            listCustomers();
            break;

        case 7:
            createInvoice();
            break;

        case 8:
            listInvoices();
            break;

        case 9:
            return;

        default:
            cout << "Invalid choice!\n";
        }
    }
}

void ShoppingCart::buyer() {
    int customerID;
    cout << "Enter your Customer ID: ";
    cin >> customerID;

    auto customerIt = find_if(customers.begin(), customers.end(),
        [customerID](const Customer& c) { return c.customerId == customerID; });

    if (customerIt != customers.end()) {
        Customer& customer = *customerIt;
        Invoice newInvoice(invoiceNumberCounter++, customer);

        while (true) {
            int productCode;
            cout << "Enter the product code to add to the invoice (0 to exit): ";
            cin >> productCode;

            if (productCode == 0) {
                break;
            }

            auto productIt = find_if(availableProducts.begin(), availableProducts.end(),
                [productCode](const Product* p) { return p->pcode == productCode; });

            if (productIt != availableProducts.end()) {
                Product* selectedProduct = *productIt;
                newInvoice.addProduct(selectedProduct);
            }
            else {
                cout << "Invalid product code. Please try again.\n";
            }
        }

        invoices.push_back(newInvoice);
        cout << "\nInvoice generated successfully.\n";
    }
    else {
        cout << "Customer not found. Please check your Customer ID.\n";
    }
}

void ShoppingCart::addProduct() {
    int choice;
    cout << "\n\n1) Electronics\n";
    cout << "2) Clothing\n";
    cout << "3) Groceries\n";
    cout << "Select the type of product to add: ";
    cin >> choice;

    int code;
    float pr, disc;
    string name, brandOrSizeOrCat;

    cout << "Enter Product Code: ";
    cin >> code;
    cout << "Enter Product Name: ";
    cin >> name;
    cout << "Enter Product Price: ";
    cin >> pr;
    cout << "Enter Product Discount: ";
    cin >> disc;

    switch (choice) {
    case 1:
        cout << "Enter Electronics Brand: ";
        cin >> brandOrSizeOrCat;
        availableProducts.push_back(new Electronics(code, pr, disc, name, brandOrSizeOrCat));
        break;

    case 2:
        cout << "Enter Clothing Size: ";
        cin >> brandOrSizeOrCat;
        availableProducts.push_back(new Clothing(code, pr, disc, name, brandOrSizeOrCat));
        break;

    case 3:
        cout << "Enter Groceries Category: ";
        cin >> brandOrSizeOrCat;
        availableProducts.push_back(new Groceries(code, pr, disc, name, brandOrSizeOrCat));
        break;

    default:
        cout << "Invalid choice.\n";
        return;
    }

    cout << "Product added successfully.\n";
}

void ShoppingCart::editProduct() {
    int code;
    cout << "Enter Product Code to edit: ";
    cin >> code;

    auto productIt = find_if(availableProducts.begin(), availableProducts.end(),
        [code](const Product* p) { return p->pcode == code; });

    if (productIt != availableProducts.end()) {
        Product* product = *productIt;
        product->display();

        // Assuming you can edit the product name, price, and discount
        cout << "\nEnter new Product Name: ";
        cin >> product->pname;
        cout << "Enter new Product Price: ";
        cin >> product->price;
        cout << "Enter new Product Discount: ";
        cin >> product->discount;

        cout << "Product edited successfully.\n";
    }
    else {
        cout << "Product not found. Please check the product code.\n";
    }
}

void ShoppingCart::removeProduct() {
    int code;
    cout << "Enter Product Code to remove: ";
    cin >> code;

    auto productIt = remove_if(availableProducts.begin(), availableProducts.end(),
        [code](const Product* p) { return p->pcode == code; });

    if (productIt != availableProducts.end()) {
        availableProducts.erase(productIt, availableProducts.end());
        cout << "Product removed successfully.\n";
    }
    else {
        cout << "Product not found. Please check the product code.\n";
    }
}

void ShoppingCart::listProducts() const {
    cout << "\nList of Available Products:\n";
    for (const auto& product : availableProducts) {
        product->display();
        cout << "\n";
    }
}

void ShoppingCart::addCustomer() {
    string name;
    cout << "Enter Customer Name: ";
    cin >> name;

    customers.push_back(Customer(name, customerIDCounter++));
    cout << "Customer added successfully.\n";
}

void ShoppingCart::listCustomers() const {
    cout << "\nList of Customers:\n";
    for (const auto& customer : customers) {
        cout << "Customer ID: " << customer.customerId << ", Name: " << customer.name << "\n";
    }
}

void ShoppingCart::createInvoice() {
    int customerID;
    cout << "Enter Customer ID to create an invoice: ";
    cin >> customerID;

    auto customerIt = find_if(customers.begin(), customers.end(),
        [customerID](const Customer& c) { return c.customerId == customerID; });

    if (customerIt != customers.end()) {
        Customer& customer = *customerIt;
        Invoice newInvoice(invoiceNumberCounter++, customer);

        while (true) {
            int productCode;
            cout << "Enter the product code to add to the invoice (0 to exit): ";
            cin >> productCode;

            if (productCode == 0) {
                break;
            }

            auto productIt = find_if(availableProducts.begin(), availableProducts.end(),
                [productCode](const Product* p) { return p->pcode == productCode; });

            if (productIt != availableProducts.end()) {
                Product* selectedProduct = *productIt;
                newInvoice.addProduct(selectedProduct);
            }
            else {
                cout << "Invalid product code. Please try again.\n";
            }
        }

        invoices.push_back(newInvoice);
        cout << "\nInvoice created successfully.\n";
    }
    else {
        cout << "Customer not found. Please check your Customer ID.\n";
    }
}

void ShoppingCart::listInvoices() const {
    cout << "\nList of Invoices:\n";
    for (const auto& invoice : invoices) {
        invoice.generateInvoice();
        cout << "\n----------------------------------------\n";
    }
}

int main() {
    ShoppingCart cart;
    cart.menu();

    return 0;
}
