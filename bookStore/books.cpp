#include "books.hpp"

int books::count(){
    CON;
    int counter = 0;
    if (con) {
        Statement *stmt = con->createStatement();
        ResultSet *rset = stmt->executeQuery("SELECT count(*) as cnt from TheStoreBooks");
        rset->first();
        counter = rset->getInt("cnt");
        delete con;
        delete rset;
        delete stmt;
    }
    return counter;
    }

void books::inventory () {
    CON;
    if (con) {
        Statement *stmt = con->createStatement();
        ResultSet *rset = stmt->executeQuery("SELECT Book_Name,Category,Book_Language,Author,Publishing_Year FROM Books INNER JOIN TheStoreBooks ON TheStoreBooks.ISBN=Books.ISBN;");
        cout << "\tBook Name\t\t\t" << "Category\t\t\t" << "Language\t\t\t" << "Autor\t\t\t" << "Publishing Year\t\t\t" << endl;
        while (rset->next()){
            cout << "\t" << rset->getString(1) << "\t" <<
            rset->getString(2) << "\t" << rset->getString(3) << "\t" << rset->getString(4) << "\t" <<
            rset->getString(5) << endl;
        }
        cout << "\n";
        delete con;
        delete rset;
        delete stmt;
    }
}

void books::orders(){
    CON;
    if (con) {
        Statement *stmt = con->createStatement();
        ResultSet *rset = stmt->executeQuery("SELECT * FROM Orders WHERE Order_Status != 'cancelled' && Order_Status != 'Purchased';");
        cout << "\tOrder Number\t\t\t" << "Customer ID\t\t\t" << "Employee ID\t\t\t" << "Order Status\t\t\t" << "Order Date\t\t\t" << "Total Price\t\t\t" << endl;
        while (rset->next()){
            cout << "\t" << rset->getInt(1) << "\t" <<
            rset->getString(2) << "\t" << rset->getString(3) << "\t" << rset->getString(4) << "\t" <<
            rset->getString(5) << rset->getString(6) << endl;
        }
        cout << "\n";
        delete con;
        delete rset;
        delete stmt;
    }
}

void books::ordersByDate(){
    CON;
    if (con) {
    
    string tmp1, tmp2;
        
    PreparedStatement *pstmt = con->prepareStatement("SELECT * FROM orders where order_date >= ? AND order_date <= ?;");
    
    cout << "Please Enter Start Date in the following format- YYYY-MM-DD > ";
    string temp;
    getline(cin, temp);
    getline(cin, tmp1);
    pstmt->setString(1, tmp1);
    cout << "Please Enter End Date in the following format- YYYY-MM-DD > ";
    getline(cin, tmp2);
    pstmt->setString(2, tmp2);
    ResultSet *rset = pstmt->executeQuery();
     
    if (rset->getRow()){
            cout << "\n\tOrder Number\t\t\t" << "Customer ID\t\t\t" << "Employee ID\t\t\t" << "Order Status\t\t\t" << "Order Date\t\t\t" << "Total Price\t\t\t" << endl;
        while (rset->next()){
            cout << "\t" << rset->getInt(1) << "\t" <<
            rset->getString(2) << "\t" << rset->getString(3) << "\t" << rset->getString(4) << "\t" <<
            rset->getString(5) << "\t" << rset->getString(6) << endl;
        }
        cout << "\n";
    }
    else
        cout << "\nInvalid Dates Or No orders for this Range\n" << endl;
    }
}

void books::DiscuntedBooks(){
    CON;
    if (con) {
        Statement *stmt = con->createStatement();
        ResultSet *rset = stmt->executeQuery("SELECT Book_Name,Category,Book_Language,Author,Publishing_Year,price,Global_Discount FROM Books INNER JOIN TheStoreBooks ON TheStoreBooks.ISBN=Books.ISBN WHERE Global_Discount>0;");
        cout << "\tBook Name\t\t\t" << "Category\t\t" << "Language\t\t" << "Autor\t\t" << "Publishing Year\t\t" << "Discount\t\t" << "Price After Discount" << endl;
        
        while (rset->next()){
            float dis = stof(rset->getString(7));
            float price = stof(rset->getString(6));
            float priseAfterDis = price-(price*dis/100);
            cout << "\t" << rset->getString(1) << "\t" <<
            rset->getString(2) << "\t" << rset->getString(3) << "\t" << rset->getString(4) << "\t" <<
            rset->getString(5) << "\t" << dis*100 << "%" << "\t" << priseAfterDis << endl;
        }
        cout << "\n";
        delete con;
        delete rset;
        delete stmt;
    }
}

void books::bookSearching(){
    CON;
    if (con) {
        
        string tmp1, tmp2;
        
        PreparedStatement *pstmt = con->prepareStatement("SELECT Book_Name,Author,Books_Amount FROM Books INNER JOIN TheStoreBooks ON TheStoreBooks.ISBN=Books.ISBN WHERE Book_Name=? OR Author=?;");
        
        cout << "Please Enter Book Name > ";
        string temp;
        getline(cin, temp);
        getline(cin, tmp1);
        pstmt->setString(1, tmp1);
        cout << "Please Enter Author Name > ";
        getline(cin, tmp2);
        pstmt->setString(2, tmp2);
        ResultSet *rset = pstmt->executeQuery();
        
        if (rset->first()){
            cout << "\n" << rset->getString(3) << " Books Exists in Inventory" << endl;
            cout << "\n";
        }
        else
            cout << "\nBook not Exists in Inventory\n" << endl;
    }
}

void books::booksSince(){
    CON;
    if (con) {
        
        string BookName, Date;
        
        PreparedStatement *pstmt = con->prepareStatement("select Book_Name from Orders natural join OrdersContent where order_date >=? AND Book_Name=?;");
        
        cout << "Please Enter Book Name > ";
        string temp;
        getline(cin, temp);
        getline(cin, BookName);
        pstmt->setString(1, Date);
        cout << "Please Enter Start Date in the following format- YYYY-MM-DD > ";
        getline(cin, Date);
        pstmt->setString(2, BookName);
        ResultSet *rset = pstmt->executeQuery();
        
        rset->beforeFirst();
        if (rset->next())
            cout << "\nThe Book '" << BookName << "' Was Ordered " << rset->rowsCount() << " Times Since " << Date << "\n" << endl;
        else
            cout << "\nBook not Exists in Inventory Or Wasn't Sold since " << Date << "\n" << endl;
    }
}

void books::OrdersAmount(){
    CON;
    if (con) {
        
        string startDate, endDate;
        
        PreparedStatement *pstmt = con->prepareStatement("SELECT * from Orders WHERE Order_Date>=? AND Order_Date<=? AND Order_Status!='Cancelled';");
        
        cout << "Please Enter Start Date in the following format- YYYY-MM-DD > ";
        string temp;
        getline(cin, temp);
        getline(cin, startDate);
        pstmt->setString(1, startDate);
        cout << "Please Enter Start Date in the following format- YYYY-MM-DD > ";
        getline(cin, endDate);
        pstmt->setString(2, endDate);
        ResultSet *rset = pstmt->executeQuery();
        
        rset->beforeFirst();
        if (rset->next())
            cout << "\n" << rset->rowsCount() <<  " Orders Between " << startDate << " and " << endDate << "\n" << endl;
        else
            cout << "\nInvalid Input or no orders in this range of dates " << "\n" << endl;
    }
}

void books::PhurchacesAmount(){
    CON;
    if (con) {
        
        string startDate, endDate;
        
        PreparedStatement *pstmt = con->prepareStatement("SELECT * from Orders WHERE Order_Date>=? AND Order_Date<=? AND Order_Status='Purchased';");
        
        cout << "Please Enter Start Date in the following format- YYYY-MM-DD > ";
        string temp;
        getline(cin, temp);
        getline(cin, startDate);
        pstmt->setString(1, startDate);
        cout << "Please Enter Start Date in the following format- YYYY-MM-DD > ";
        getline(cin, endDate);
        pstmt->setString(2, endDate);
        ResultSet *rset = pstmt->executeQuery();
        
        rset->beforeFirst();
        if (rset->next())
            cout << "\n" << rset->rowsCount() <<  " Purchases Between " << startDate << " and " << endDate << "\n" << endl;
        else
            cout << "\nInvalid Input or no orders in this range of dates " << "\n" << endl;
    }
}

void books::top10Books(){
    CON;
    if (con) {
        
        string startDate, endDate;
        int counter = 1;
        
        PreparedStatement *pstmt = con->prepareStatement("SELECT Book_Name, COUNT(*) AS book_count FROM orders natural join OrdersContent WHERE Order_Date >= ? AND Order_Date <= ? AND Order_Status!= 'Cancelled' group by Book_Name ORDER BY book_count DESC;");
        
        cout << "Please Enter Start Date in the following format- YYYY-MM-DD > ";
        string temp;
        getline(cin, temp);
        getline(cin, startDate);
        pstmt->setString(1, startDate);
        
        cout << "Please Enter Start Date in the following format- YYYY-MM-DD > ";
        getline(cin, endDate);
        pstmt->setString(2, endDate);
        
        ResultSet *rset = pstmt->executeQuery();
        
        if (rset->rowsCount() == 0) {
            cout << "Invaid Input Or no orders in this range of dates " << endl;
            return;
        }
        
        cout << "\nTop Books Sold Between " << startDate << " And " << endDate << " -" << endl;
        rset->beforeFirst();
        while (rset->next()) {
            cout << counter << ". " << rset->getString("Book_Name") << " - " <<  rset->getString("book_count") << " Copys" << endl;
            counter++;
        }
        
        cout << "\n";
    }
}


