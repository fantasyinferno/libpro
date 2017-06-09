drop table if exists status;
create table status(status_id INTEGER PRIMARY KEY, status VARCHAR(25));
INSERT INTO status(status) VALUES("Hoạt động"), ("Bị khóa");