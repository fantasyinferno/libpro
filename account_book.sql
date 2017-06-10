drop table if exists account_book;
create table account_book(account_id INTEGER, book_id CHAR(15), start_date DATE, due_date DATE, PRIMARY KEY (account_id, book_id));
insert into account_book VALUES (1, '0262033844', '2017/1/1', '2017/2/1');