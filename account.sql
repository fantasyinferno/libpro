drop table if exists account;
create table account(account_id INTEGER PRIMARY KEY, account VARCHAR(255), password BLOB, status_id INTEGER, fullname VARCHAR(255), identity_number CHAR(10), gender_id INTEGER, birthdate DATE, email VARCHAR(255), job VARCHAR(50));
insert into account(account, password, status_id, fullname, identity_number, gender_id, birthdate, email, job) VALUES ('conbocuoi', '123456', 1, 'Nguyễn Hồ Minh Phước', '01234567', 1, '2017-01-01', 'conbocuoi@hotmail.com', 'Lao động');