CREATE TABLE csci399_ass2 (
	name varchar(8) PRIMARY KEY,
	passwd varchar(8) NOT NULL,
	played integer DEFAULT 0,
	won integer DEFAULT 0 CONSTRAINT legally_won CHECK (won <= played)
);
