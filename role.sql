drop table if exists role;
create table role(role_id INT, role CHAR(50), PRIMARY KEY (role_id));
insert into role(role) values ('Người đọc'), ('Thủ thư'), ('Quản lý');