This project is developed in LINUX(Ubuntu) environment. So, run the following commands in terminal in order to run the project:-

1.  sudo apt-get install libmysqlclient-dev  //install before compiling
2.  gcc tours_n_travel.c -o tours_n_travel -lmysqlclient //For compilation, use this statement to connect with mysql
3.  sudo ./tours_n_travel   //use this statement for execution execution


MySQL Scripts which are used to create database and tables which are used in this project.
1.  To create the database

    create database travel_management;
    use travel_management;
    
    
2.  To create the tables
    
    CREATE TABLE `route` (
    `id` BIGINT(20) NOT NULL AUTO_INCREMENT,
    `route_name` VARCHAR(60) DEFAULT NULL,
    `from` VARCHAR(200) DEFAULT NULL,
    `to` VARCHAR(200) DEFAULT NULL,
    `fair_per_person` INT DEFAULT 0,
    time_of_departure DATETIME,
    PRIMARY KEY (`id`)
    )
    ENGINE=InnoDB
    AUTO_INCREMENT = 1;
    
    CREATE TABLE `purchase` (
    `id` BIGINT(20) NOT NULL AUTO_INCREMENT,
    `customer_name` VARCHAR(60) DEFAULT NULL,
    `phone_number` VARCHAR(10) DEFAULT NULL,
    `number_of_seats` INT DEFAULT 1,
    `total_price` INT DEFAULT 0,
    `route_id` BIGINT(20) NOT NULL,
    time_of_booking DATETIME,
    PRIMARY KEY (`id`),
    KEY `fk_route_1` (`route_id`),
    CONSTRAINT `fk_route_1` FOREIGN KEY 
    (`route_id`) REFERENCES `route`(`id`)
    )
    ENGINE=InnoDB
    AUTO_INCREMENT = 1;
    
    CREATE TABLE `seat`(
    `id` BIGINT(20) NOT NULL AUTO_INCREMENT,
    `seat_number` VARCHAR(6) DEFAULT NULL,
    `route_id` BIGINT(20) NOT NULL,
    `available` boolean NOT NULL DEFAULT 1,
    `pid` BIGINT(20) DEFAULT NULL,
    PRIMARY KEY (`id`),
    KEY `fk_route_2` (`route_id`),
    KEY `fk_purchase` (`pid`),
    CONSTRAINT `fk_route_2` FOREIGN KEY 
    (`route_id`) REFERENCES `route`(`id`),
    CONSTRAINT `fk_purchase` FOREIGN KEY 
    (`pid`) REFERENCES `purchase`(`id`)
    )
    ENGINE=InnoDB
    AUTO_INCREMENT = 1;


3.  To Insert dummy data into the tables
    
    INSERT INTO route(`route_name`,`from`,`to`,`fair_per_person`,time_of_departure) VALUES('Kedarnath Yatra','Kolkata','Kedarnath',5000,’2022-02-05 18:00:00’);
    INSERT INTO route(`route_name`,`from`,`to`,`fair_per_person`) VALUES('Puri Rath Yatra','Kolkata','Puri',1500, ’2022-02-12 18:00:00);
    INSERT INTO route(`route_name`,`from`,`to`,`fair_per_person`) VALUES('Queen of Hills','Kolkata','Mussoorie',5500, ’2022-02-19 18:00:00);
    
    INSERT INTO seat(seat_number,route_id) VALUES(‘S0001’,1 );
    INSERT INTO seat(seat_number,route_id) VALUES(‘S0002’,1);
    INSERT INTO seat(seat_number,route_id) VALUES(‘S0003’,1 );
    INSERT INTO seat(seat_number,route_id) VALUES(‘S0004’,1);
    INSERT INTO seat(seat_number,route_id) VALUES(‘S0001’,2 );
    INSERT INTO seat(seat_number,route_id) VALUES(‘S0002’,2);
    INSERT INTO seat(seat_number,route_id) VALUES(‘S0003’,2);
    INSERT INTO seat(seat_number,route_id) VALUES(‘S0004’,2);
    INSERT INTO seat(seat_number,route_id) VALUES(‘S0001’,3 );
    INSERT INTO seat(seat_number,route_id) VALUES(‘S0002’,3);
    INSERT INTO seat(seat_number,route_id) VALUES(‘S0003’,3 );
    INSERT INTO seat(seat_number,route_id) VALUES(‘S0004’,3);
