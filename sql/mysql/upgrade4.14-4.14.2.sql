ALTER TABLE  `salesman` ADD  `address1` VARCHAR( 30 ) NOT NULL default '0';
ALTER TABLE  `salesman` ADD  `address2` VARCHAR( 30 ) NOT NULL default '0';
ALTER TABLE  `salesman` ADD  `address3` VARCHAR( 30 ) NOT NULL default '0';
ALTER TABLE  `salesman` ADD  `address4` VARCHAR( 30 ) NOT NULL default '0';
ALTER TABLE  `salesman` ADD  `address5` VARCHAR( 30 ) NOT NULL default '0';
ALTER TABLE `debtorsmaster` ADD `phone_number` bigint(20) NOT NULL default '0';
ALTER TABLE `debtorsmaster` ADD `email_id` varchar(30) NOT NULL default '0';