drop table if exists book_status;
CREATE TABLE book_status(book_status_id INTEGER PRIMARY KEY, book_status VARCHAR(50));
INSERT INTO book_status(book_status) VALUES ("Chờ duyệt"), ("Đang mượn"), ("Đã trả"), ("Bị mất");