#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <stack>
#include <queue>

#define ANSI_RESET "\x1B[0m"
#define ANSI_RED "\x1B[91m"
#define ANSI_GREEN "\x1B[92m"
#define ANSI_YELLOW "\x1B[33m"
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
            cout << "Book '" << book.title << "' added to reservation queue.\n";
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
            cout << ANSI_CYAN << "Library Management System" << ANSI_RESET << "\n";
            cout << ANSI_YELLOW << "1. User\n";
            cout << "2. Admin\n";
            cout << "3. Exit\n" << ANSI_RESET;
            cout << "Enter your choice: ";
            cin >> choice;

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
                    cout << ANSI_GREEN << "Goodbye!" << ANSI_RESET << "\n";
                    break;
                default:
                    cout << ANSI_RED << "Invalid choice. Try again." << ANSI_RESET << "\n";
                    break;
            }
        } while (choice != 3);
    }

    void userMenu()
    {
        int choice;

        do {
            cout << "\nUser Menu\n";
            cout << ANSI_YELLOW << "1. View available books\n";
            cout << "2. Issue a book\n";
            cout << "3. Return a book\n";
            cout << "4. View issued books\n";
            cout << "5. Return to the main menu\n" << ANSI_RESET;
            cout << "Enter your choice: ";
            cin >> choice;

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
                    viewIssuedBooks();
                    break;
                case 5:
                    return;
                default:
                    cout << ANSI_RED << "Invalid choice. Try again." << ANSI_RESET << "\n";
                    break;
            }
        } while (choice != 5);
    }

    void viewIssuedBooks()
    {
        if (issuedBooks.empty())
        {
            cout << "You haven't issued any books.\n";
        }
        else
        {
            reindexIssuedBooks();
            cout << "\nIssued Books:\n";

            for (const Book& book : issuedBooks)
            {
                displayBook(book);
            }
        }
    }

    void adminMenu()
    {
        int choice;

        do {
            cout << "\nAdmin Menu\n";
            cout << ANSI_YELLOW << "1. View all books\n";
            cout << "2. Add a book\n";
            cout << "3. Update a book\n";
            cout << "4. Delete a book\n";
            cout << "5. Return to the main menu\n" << ANSI_RESET;
            cout << "Enter your choice: ";
            cin >> choice;

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
                    cout << ANSI_RED << "Invalid choice. Try again." << ANSI_RESET << "\n";
                    break;
            }
        } while (choice != 5);
    }

    void viewAvailableBooks()
    {
        cout << "\nAvailable Books:\n";

        for (const Book& book : books)
        {
            if (book.available)
            {
                displayBook(book);
            }
        }
    }

    void viewAllBooks()
    {
        cout << "\nAll Books:\n";

        for (const Book& book : books)
        {
            displayBook(book);
        }
    }

    void issueBook()
    {
        int id;
        cout << "Enter the book ID to issue: ";
        cin >> id;

        for (Book& book : books)
        {
            if (book.id == id && book.available)
            {
                book.available = false;

                Book issuedBook = book;
                issuedBooks.push_back(issuedBook);
                reindexIssuedBooks();

                books.erase(remove(books.begin(), books.end(), book), books.end());
                reindexMainBooks();

                cout << ANSI_GREEN << "Book issued successfully." << ANSI_RESET << "\n";
                return;
            }
        }

        cout << ANSI_RED << "Book not found or already issued." << ANSI_RESET << "\n";
    }

    void returnBook()
    {
        int id;

        cout << "Enter the book ID to return: ";
        cin >> id;

        auto it = find_if(issuedBooks.begin(), issuedBooks.end(), [&id](const Book& book) { return book.id == id; });

        if (it != issuedBooks.end())
        {
            Book book = *it;
            book.available = true;
            books.push_back(book);
            issuedBooks.erase(it);

            reindexMainBooks();
            reindexIssuedBooks();

            cout << ANSI_GREEN << "Book returned successfully." << ANSI_RESET << "\n";
        }
        else
        {
            cout << ANSI_RED << "Book not found in issued books." << ANSI_RESET << "\n";
        }
    }

    // New method to check for duplicate books
    bool isDuplicateBook(const string& title, const string& author)
    {
        for (const Book& book : books)
        {
            if (book.title == title && book.author == author)
            {
                return true;
            }
        }
        return false;
    }

    void addBook()
    {
        Book newBook;
        newBook.id = getNextBookID();

        // Input validation for title
        while (true) {
            cout << "Enter the book title: ";
            cin.ignore();
            getline(cin, newBook.title);
            if (!newBook.title.empty()) break;
            cout << ANSI_RED << "Title cannot be empty. Please try again." << ANSI_RESET << endl;
        }

        // Input validation for author
        while (true) {
            cout << "Enter the book author: ";
            getline(cin, newBook.author);
            if (!newBook.author.empty()) break;
            cout << ANSI_RED << "Author cannot be empty. Please try again." << ANSI_RESET << endl;
        }

        // Check for duplicate book
        if (isDuplicateBook(newBook.title, newBook.author))
        {
            cout << ANSI_RED << "A book with the same title and author already exists." << ANSI_RESET << endl;
            return;
        }

        newBook.available = true;
        books.push_back(newBook);
        rebuildBookIndex();
        cout << ANSI_GREEN << "Book added successfully." << ANSI_RESET << endl;
    }

    void updateBook()
    {
        int id;

        cout << "Enter the book ID to update: ";
        cin >> id;

        auto it = bookIndex.find(id);

        if (it != bookIndex.end())
        {
            Book* book = it->second;

            cout << "Enter the new title of the book: ";
            cin.ignore();
            getline(cin, book->title);

            cout << "Enter the new author of the book: ";
            getline(cin, book->author);

            bookOperationStack.pushOperation("Update", *book);

            cout << ANSI_GREEN << "Book updated successfully." << ANSI_RESET << "\n";
        }
        else
        {
            cout << ANSI_RED << "Book not found." << ANSI_RESET << "\n";
        }
    }

    void deleteBook()
    {
        int id;

        cout << "Enter the book ID to delete: ";
        cin >> id;

        auto it = bookIndex.find(id);

        if (it != bookIndex.end())
        {
            Book* book = it->second;
            books.erase(remove(books.begin(), books.end(), *book), books.end());
            reindexMainBooks();
            cout << ANSI_GREEN << "Book deleted successfully." << ANSI_RESET << "\n";
        }
        else
        {
            cout << ANSI_RED << "Book not found." << ANSI_RESET << "\n";
        }
    }

    void displayBook(const Book& book)
    {
        cout << "ID: " << book.id << ", Title: " << book.title << ", Author: " << book.author << ", Available: " << (book.available ? "Yes" : "No") << "\n";
    }

    int getNextBookID()
    {
        return books.empty() ? 1 : books.back().id + 1;
    }

    void loadBooks()
    {
        books.push_back({"Howl's Moving Castle", "Diana Wynne Jones", 1, true});
        books.push_back({"Spirited Away", "Hayao Miyazaki", 2, true});
        books.push_back({"The Tale of the Princess Kaguya", "Isao Takahata", 3, true});
    }

    void loadIssuedBooks()
    {
        issuedBooks.push_back({"Ponyo", "Hayao Miyazaki", 1, false});
    }

    void saveBooks()
    {
        ofstream outputFile("books.txt");
        if (outputFile.is_open())
        {
            for (const Book& book : books)
            {
                outputFile << book.id << "\n";
                outputFile << book.title << "\n";
                outputFile << book.author << "\n";
                outputFile << book.available << "\n";
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
            }
            outputFile.close();
        }
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
