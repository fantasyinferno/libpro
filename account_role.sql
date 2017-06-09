drop table if exists account_role;
create table account_role(account_id INTEGER, role_id INTEGER, PRIMARY KEY(account_id, role_id), FOREIGN KEY (account_id) REFERENCES account(account_id), FOREIGN KEY (role_id) REFERENCES role(role_id));
insert into account_role(account_id, role_id) values (1,1), (1,2);