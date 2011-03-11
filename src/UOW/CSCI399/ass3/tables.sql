CREATE TABLE patients (
	id integer PRIMARY KEY,
	surname varchar(32) NOT NULL,
	initials varchar(8),
	age integer CONSTRAINT positive_age CHECK (age > 0),
	gender char(1) NOT NULL,
	height real CONSTRAINT positive_height CHECK (height > 0)
);

CREATE TABLE consultations (
	consultation_date date NOT NULL,
	weight real CONSTRAINT positive_weight CHECK (weight > 0),
	patient_id integer REFERENCES patients (id) ON DELETE CASCADE
);
