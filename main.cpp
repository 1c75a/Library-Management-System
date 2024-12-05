#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <stack>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <iomanip>

#define ANSI_RESET "\x1B[0m"
#define ANSI_RED "\x1B[91m"
#define ANSI_GREEN "\x1B[92m"
#define ANSI_BLUE "\x1B[94m"
#define ANSI_MAGENTA "\x1B[95m"
#define ANSI_CYAN "\x1B[96m"

using namespace std;

struct Book
{
    string title;
    string author;
    int id;
    bool available;
    string borrowerName;
    string borrowDate;

    bool operator>(const Book& other) const
    {
        return id > other.id;
    }
    bool operator==(const Book& other) const
    {
        return id == other.id && title == other.title && author == other.author;
    }
};

class Library
{
public:
    Library()
    {
        loadBooks();
        loadIssuedBooks();
        rebuildBookIndex();
    }

    void bubbleSort(vector<Book>& bookList, bool ascending = true)
    {
        int n = bookList.size();

        for (int i = 0; i < n - 1; i++)
        {
            for (int j = 0; j < n - i - 1; j++)
            {
                bool shouldSwap = ascending ?
                    (bookList[j].id > bookList[j + 1].id) :
                    (bookList[j].id < bookList[j + 1].id);

                if (shouldSwap)
                {
                    swap(bookList[j], bookList[j + 1]);
                }
            }
        }
    }

    void reindexBooks(vector<Book>& bookList)
    {
        bubbleSort(bookList);

        for (size_t i = 0; i < bookList.size(); ++i)
        {
            bookList[i].id = i + 1;
        }
    }

    void reindexMainBooks()
    {
        reindexBooks(books);
        rebuildBookIndex();
    }

    void reindexIssuedBooks()
    {
        reindexBooks(issuedBooks);
    }

    int binarySearchBooks(int targetId)
    {
        vector<Book> sortedBooks = books;
        bubbleSort(sortedBooks);

        int left = 0;
        int right = sortedBooks.size() - 1;

        while (left <= right)
        {
            int mid = left + (right - left) / 2;

            if (sortedBooks[mid].id == targetId)
                return mid;

            if (sortedBooks[mid].id < targetId)
                left = mid + 1;
            else
                right = mid - 1;
        }

        return -1;
    }

    class BookOperationStack
    {
    private:
        stack<pair<string, Book>> operationStack;

    public:
        void pushOperation(const string& operation, const Book& book)
        {
            operationStack.push({operation, book});
        }

        pair<string, Book> popOperation()
        {
            if (!operationStack.empty())
            {
                auto top = operationStack.top();
                operationStack.pop();
                return top;
            }

            return {"", Book()};
        }

        bool isEmpty()
        {
            return operationStack.empty();
        }
    };

    class BookReservationQueue
    {
    private:
        queue<Book> reservationQueue;

    public:
        void enqueueReservation(const Book& book)
        {
            reservationQueue.push(book);

            cout<<"Book '"<<book.title<<"' added to reservation queue.\n";
        }

        Book dequeueReservation()
        {
            if (!reservationQueue.empty())
            {
                Book book = reservationQueue.front();
                reservationQueue.pop();
                return book;
            }

            return Book();
        }

        int queueSize()
        {
            return reservationQueue.size();
        }
    };

    unordered_map<int, Book*> bookIndex;

    void rebuildBookIndex()
    {
        bookIndex.clear();
        for (Book& book : books)
        {
            bookIndex[book.id] = &book;
        }
    }

    void displayMenu()
    {
        int choice;

        do {
            system("cls");
            cout<<ANSI_CYAN<<"===================================================================================================================================================="<<ANSI_RESET<<"\n\n";
            cout<<ANSI_CYAN<<"                                                                 DDDD    OOO    Y   Y  " <<ANSI_RESET<<"\n";
            cout<<ANSI_CYAN<<"                                                                 D   D  O   O    Y Y   " <<ANSI_RESET<<"\n";
            cout<<ANSI_CYAN<<"                                                                 D   D  O   O     Y    " <<ANSI_RESET<<"\n";
            cout<<ANSI_CYAN<<"                                                                 D   D  O   O     Y    " <<ANSI_RESET<<"\n";
            cout<<ANSI_CYAN<<"                                                                 D   D  O   O     Y    " <<ANSI_RESET<<"\n";
            cout<<ANSI_CYAN<<"                                                                 D   D  O   O     Y    " <<ANSI_RESET<<"\n";
            cout<<ANSI_CYAN<<"                                                                 DDDD    OOO      Y    " <<ANSI_RESET<<"\n\n";
            cout<<ANSI_CYAN<<"===================================================================================================================================================="<<ANSI_RESET<<"\n\n";
            cout<<ANSI_CYAN<<"\nLIBRARY MANAGEMENT SYSTEM:\n\n";
            cout<<ANSI_CYAN<<"1. User\n";
            cout<<"2. Admin\n";
            cout<<ANSI_RED<<"3. Exit\n"<<ANSI_RESET<<"\n";
            cout<<ANSI_CYAN<<"Enter your choice: ";
            cin>>choice;

            switch (choice)
            {
                case 1:
                    userMenu();
                    break;
                case 2:
                    adminMenu();
                    break;
                case 3:
                    saveBooks();
                    saveIssuedBooks();
                    cout<<ANSI_GREEN<<"\nGoodbye!"<<ANSI_RESET<<"\n";
                    break;
                default:
                    cout<<ANSI_RED<<"Invalid choice. Try again."<<ANSI_RESET<<"\n";
                    break;
            }
        } while (choice != 3);
    }

    void userMenu()
    {
        int choice;

        do {
            system("cls");
            cout<<ANSI_CYAN<<"===================================================================================================================================================="<<ANSI_RESET<<"\n\n";
            cout<<ANSI_CYAN<<"                                                                 DDDD    OOO    Y   Y  "<<ANSI_RESET<<"\n";
            cout<<ANSI_CYAN<<"                                                                 D   D  O   O    Y Y   "<<ANSI_RESET<<"\n";
            cout<<ANSI_CYAN<<"                                                                 D   D  O   O     Y    "<<ANSI_RESET<<"\n";
            cout<<ANSI_CYAN<<"                                                                 D   D  O   O     Y    "<<ANSI_RESET<<"\n";
            cout<<ANSI_CYAN<<"                                                                 D   D  O   O     Y    "<<ANSI_RESET<<"\n";
            cout<<ANSI_CYAN<<"                                                                 D   D  O   O     Y    "<<ANSI_RESET<<"\n";
            cout<<ANSI_CYAN<<"                                                                 DDDD    OOO      Y    "<<ANSI_RESET<<"\n\n";
            cout<<ANSI_CYAN<<"===================================================================================================================================================="<<ANSI_RESET<<"\n\n";
            cout<<ANSI_CYAN<<"\nUser Menu\n\n";
            cout<<ANSI_CYAN<<"1. View available books\n";
            cout<<ANSI_CYAN<<"2. Issue a book\n";
            cout<<ANSI_CYAN<<"3. Return a book\n";
            cout<<ANSI_CYAN<<"4. View issued books\n";
            cout<<ANSI_CYAN<<"5. Return to the main menu\n"<<ANSI_RESET<<"\n";
            cout<<ANSI_CYAN<<"Enter your choice: ";
            cin>>choice;

            switch (choice)
            {
                case 1:
                    viewAvailableBooks();
                    break;
                case 2:
                    issueBook();
                    break;
                case 3:
                    returnBook();
                    break;
                case 4:
                    displayIssuedBooks();
                    break;
                case 5:
                    return;
                default:
                    cout<<ANSI_RED<<"Invalid choice. Try again."<<ANSI_RESET<<"\n";
                    break;
            }
        } while (choice != 5);
    }

    void viewAvailableBooks()
    {
        cout<<ANSI_CYAN<<"\nAvailable Books:\n\n";

        for (const Book& book : books)
        {
            if (book.available)
            {
                displayBook(book);
            }
        }

        cout<<ANSI_CYAN<<"\nPress Enter to continue...";
        cin.ignore();
        cin.get();
    }

    void viewAllBooks()
    {
        cout<<ANSI_CYAN<<"\nAll Books:\n\n";

        for (const Book& book : books)
        {
            displayBook(book);
        }

        cout<<ANSI_CYAN<<"\nPress Enter to continue...";
        cin.ignore();
        cin.get();
    }

    void adminMenu()
    {
        int choice;

        do {
            system("cls");
            cout<<ANSI_CYAN<<"===================================================================================================================================================="<<ANSI_RESET<<"\n\n";
            cout<<ANSI_CYAN<<"                                                                 DDDD    OOO    Y   Y  "<<ANSI_RESET<<"\n";
            cout<<ANSI_CYAN<<"                                                                 D   D  O   O    Y Y   "<<ANSI_RESET<<"\n";
            cout<<ANSI_CYAN<<"                                                                 D   D  O   O     Y    "<<ANSI_RESET<<"\n";
            cout<<ANSI_CYAN<<"                                                                 D   D  O   O     Y    "<<ANSI_RESET<<"\n";
            cout<<ANSI_CYAN<<"                                                                 D   D  O   O     Y    "<<ANSI_RESET<<"\n";
            cout<<ANSI_CYAN<<"                                                                 D   D  O   O     Y    "<<ANSI_RESET<<"\n";
            cout<<ANSI_CYAN<<"                                                                 DDDD    OOO      Y    "<<ANSI_RESET<<"\n\n";
            cout<<ANSI_CYAN<<"===================================================================================================================================================="<<ANSI_RESET<<"\n\n";
            cout<<ANSI_CYAN<<"\nAdmin Menu\n\n";
            cout<<ANSI_CYAN<<"1. View all books\n";
            cout<<ANSI_CYAN<<"2. Add a book\n";
            cout<<ANSI_CYAN<<"3. Update a book\n";
            cout<<ANSI_CYAN<<"4. Delete a book\n";
            cout<<ANSI_CYAN<<"5. Return to the main menu\n"<<ANSI_RESET<<"\n";
            cout<<ANSI_CYAN<<"Enter your choice: ";
            cin>>choice;

            switch (choice)
            {
                case 1:
                    viewAllBooks();
                    break;
                case 2:
                    addBook();
                    break;
                case 3:
                    updateBook();
                    break;
                case 4:
                    deleteBook();
                    break;
                case 5:
                    return;
                default:
                    cout<<ANSI_RED<<"Invalid choice. Try again."<<ANSI_RESET<<"\n";
                    break;
            }
        } while (choice != 5);
    }

    void issueBook()
    {
        int id;
        string borrowerName;
        cout<<ANSI_CYAN<<"\nEnter the book ID to issue: ";
        cin>>id;

        cin.ignore();

        cout<<ANSI_CYAN<<"Enter borrower's name: ";
        getline(cin, borrowerName);

        time_t now = time(0);
        char* dt = ctime(&now);
        string borrowDate = dt;
        borrowDate = borrowDate.substr(0, borrowDate.length() - 1);

        for (Book& book : books)
        {
            if (book.id == id && book.available)
            {
                book.available = false;
                book.borrowerName = borrowerName;
                book.borrowDate = borrowDate;

                Book issuedBook = book;
                issuedBooks.push_back(issuedBook);
                reindexIssuedBooks();

                books.erase(remove(books.begin(), books.end(), book), books.end());
                reindexMainBooks();

                cout<<ANSI_GREEN<<"\nBook issued successfully."<<ANSI_RESET<<"\n";
                cout<<ANSI_CYAN<<"\nBorrower: "<<borrowerName<<"\n";
                cout<<"Borrow Date: "<<borrowDate<<"\n\n";

                cout<<ANSI_CYAN<<"Press Enter to continue...";
                cin.get();

                return;
            }
        }

        cout<<ANSI_RED<<"\nBook not found or already issued."<<ANSI_RESET<<"\n\n";

        cout<<ANSI_CYAN<<"Press Enter to continue...";
        cin.get();
    }

    void displayIssuedBooks()
    {
        if (issuedBooks.empty())
        {
            cout<<ANSI_RED<<"You haven't issued any books.\n";
        }
        else
        {
            reindexIssuedBooks();
            cout<<ANSI_CYAN<<"\nIssued Books:\n\n";

            for (const Book& book : issuedBooks)
            {
                displayBook(book);
                cout<<ANSI_CYAN<<"Borrower: "<<book.borrowerName<<"\n";
                cout<<"Borrow Date: "<<book.borrowDate<<"\n\n"<<ANSI_RESET;
            }
        }

        cout<<ANSI_CYAN<<"\nPress Enter to continue...";
        cin.ignore();
        cin.get();
    }

    void returnBook()
    {
        int id;

        cout<<ANSI_CYAN<<"\nEnter the book ID to return: ";
        cin>>id;

        cin.ignore();

        auto it = find_if(issuedBooks.begin(), issuedBooks.end(), [&id](const Book& book) { return book.id == id; });

        if (it != issuedBooks.end())
        {
            Book book = *it;
            book.available = true;
            book.borrowerName = "";
            book.borrowDate = "";
            books.push_back(book);
            issuedBooks.erase(it);

            reindexMainBooks();
            reindexIssuedBooks();

            cout<<ANSI_GREEN<<"\nBook returned successfully."<<ANSI_RESET<<"\n\n";
        }
            else
            {
                cout<<ANSI_RED<<"\nBook not found in issued books."<<ANSI_RESET<<"\n\n";
            }

        cout<<ANSI_CYAN<<"Press Enter to continue...";
        cin.get();
    }

    void displayBook(const Book& book)
    {
        cout<<ANSI_CYAN<<"ID: "<<book.id
             <<", Title: "<<book.title
             <<", Author: "<<book.author
             <<", Available: "<<(book.available ? ANSI_GREEN "Yes" : ANSI_RED "No")<<ANSI_RESET<<"\n";
    }

    void addBook()
    {
        Book newBook;
        cout<<ANSI_CYAN<<"\nEnter book title: ";
        cin.ignore();
        getline(cin, newBook.title);

        cout<<"Enter book author: ";
        getline(cin, newBook.author);

        newBook.id = books.size() + 1;
        newBook.available = true;

        books.push_back(newBook);
        reindexMainBooks();

        cout<<ANSI_GREEN<<"\nBook added successfully!"<<ANSI_RESET<<"\n\n";
        cout<<ANSI_CYAN<<"Press Enter to continue...";
        cin.get();
    }

    void updateBook()
    {
        int id;
        cout<<ANSI_CYAN<<"\nEnter book ID to update: ";
        cin>>id;

        for (Book& book : books)
        {
            if (book.id == id)
            {
                cout<<ANSI_CYAN<<"\nCurrent book details:\n";
                displayBook(book);

                cin.ignore();
                cout<<ANSI_CYAN<<"\nEnter new title (press Enter to keep current): ";
                string newTitle;
                getline(cin, newTitle);
                if (!newTitle.empty()) book.title = newTitle;

                cout<<ANSI_CYAN<<"Enter new author (press Enter to keep current): ";
                string newAuthor;
                getline(cin, newAuthor);
                if (!newAuthor.empty()) book.author = newAuthor;

                cout<<ANSI_GREEN<<"\nBook updated successfully!"<<ANSI_RESET<<"\n\n";
                cout<< ANSI_CYAN<<"Press Enter to continue...";
                cin.get();
                return;
            }
        }

        cout << ANSI_RED<<"Book not found."<<ANSI_RESET<<"\n";
        cout << ANSI_CYAN<<"Press Enter to continue...";
        cin.ignore();
        cin.get();
    }

    void deleteBook()
    {
        int id;
        cout<<ANSI_CYAN<<"\nEnter book ID to delete: ";
        cin>>id;

        auto it = find_if(books.begin(), books.end(), [&id](const Book& book) { return book.id == id; });

        if (it != books.end())
        {
            books.erase(it);
            reindexMainBooks();

            cout<<ANSI_GREEN<<"\nBook deleted successfully!"<<ANSI_RESET<<"\n\n";
        }
            else
            {
                cout<<ANSI_RED<<"\nBook not found."<<ANSI_RESET<<"\n\n";
            }

        cout<<ANSI_CYAN<<"Press Enter to continue...";
        cin.ignore();
        cin.get();
    }

    void saveBooks()
    {
        ofstream outputFile("books.txt");
        if (outputFile.is_open())
        {
            for (const Book& book : books)
            {
                outputFile<<book.id<<"\n";
                outputFile<<book.title<<"\n";
                outputFile<<book.author<<"\n";
                outputFile<<book.available<<"\n";
                outputFile<<book.borrowerName<<"\n";
                outputFile<<book.borrowDate;
            }
            outputFile.close();
        }
    }

    void saveIssuedBooks()
    {
        ofstream outputFile("issued_books.txt");
        if (outputFile.is_open())
        {
            for (const Book& book : issuedBooks)
            {
                outputFile << book.id << "\n";
                outputFile << book.title << "\n";
                outputFile << book.author << "\n";
                outputFile << book.available << "\n";
                outputFile << book.borrowerName << "\n";
                outputFile << book.borrowDate;
            }
            outputFile.close();
        }
    }

    void loadBooks()
    {
        books.push_back({"Howl's Moving Castle", "Diana Wynne Jones", 1, true, "", ""});
        books.push_back({"Spirited Away", "Hayao Miyazaki", 2, true, "", ""});
        books.push_back({"The Tale of the Princess Kaguya", "Isao Takahata", 3, true, "", ""});
    }

    void loadIssuedBooks()
    {
        issuedBooks.push_back({"Ponyo", "Hayao Miyazaki", 4, false, "Jazmin", "Fri Dec 6 0:43:56 2024"});
    }

private:
    vector<Book> books;
    vector<Book> issuedBooks;
    BookOperationStack bookOperationStack;
    BookReservationQueue bookReservationQueue;
};

int main()
{
    Library library;
    library.displayMenu();

    return 0;
}
