drop table if exists account_book;
create table account_book(account_book_id INTEGER PRIMARY KEY, account_id INTEGER, book_id CHAR(15), start_date DATE, due_date DATE, book_status_id INTEGER);