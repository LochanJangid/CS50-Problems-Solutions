-- Keep a log of any SQL queries you execute as you solve the mystery.

-- for Understand tables and columns
.schema

-- for see description about crime scene 
SELECT *
  FROM crime_scene_reports
 WHERE month = 7 AND day = 28;

-- for read transcript carefully and find clues
SELECT name, transcript 
  FROM interviews
 WHERE month = 7 AND day = 28;

-- Create a table for suspects
CREATE TABLE suspects (
    person_id INTEGER,
    FOREIGN KEY(person_id) REFERENCES people(id)
);

-- Clue 1: The theif withdrawing some money at ATM on leggett Street this morning
--         add to suspect 
INSERT INTO suspects (person_id) 
SELECT DISTINCT people.id
  FROM people
  JOIN bank_accounts ON bank_accounts.person_id = people.id
  JOIN atm_transactions ON atm_transactions.account_number = bank_accounts.account_number
 WHERE atm_transactions.month = 7
   AND atm_transactions.day = 28
   AND atm_transactions.atm_location = 'Leggett Street'
   AND atm_transactions.transaction_type = 'withdraw';

-- Clue 2: The theif go with car (fottage: bakery parking lot, time: in 10 minutes)
--         add to suspect 
INSERT INTO suspects (person_id) 
SELECT DISTINCT people.id
  FROM people
  JOIN bakery_security_logs ON bakery_security_logs.license_plate = people.license_plate
 WHERE bakery_security_logs.month = 7 
   AND bakery_security_logs.day = 28
   AND bakery_security_logs.hour = 10
   AND bakery_security_logs.minute >= 15
   AND bakery_security_logs.minute <= 25
   AND bakery_security_logs.activity = 'exit';

-- To see suspects name
SELECT person_id, people.name
  FROM suspects
  JOIN people ON people.id = suspects.person_id;

-- Clue 3: theif are going with flight on next day of theift. 
--         add to suspect 
INSERT INTO suspects (person_id)
SELECT DISTINCT people.id
  FROM people
  JOIN passengers ON passengers.passport_number = people.passport_number
 WHERE passengers.flight_id = (
    SELECT flights.id
      FROM flights
      JOIN airports ON airports.id = flights.origin_airport_id
     WHERE flights.month = 7
       AND flights.day = 29
       AND airports.city = 'Fiftyville'
     ORDER BY flights.hour, flights.minute
     LIMIT 1
 );
-- Clue 4: on phone call (Caller)
-- add to suspect 
INSERT INTO suspects (person_id) 
SELECT  DISTINCT people.id
  FROM people
  JOIN phone_calls ON people.phone_number = phone_calls.caller
 WHERE phone_calls.month = 7 AND phone_calls.day = 28
   AND phone_calls.duration < 60;

-- most suspectable with high clues  (May be Thief)
SELECT people.name AS 'The THIEF is'
  FROM suspects
  JOIN people ON people.id = suspects.person_id
GROUP BY person_id
ORDER BY COUNT(*) DESC
LIMIT 1;

-- Check where thief go
SELECT city As 'The city the thief ESCAPED TO'
  FROM airports
  JOIN flights ON airports.id = flights.destination_airport_id
  JOIN passengers ON flights.id = passengers.flight_id
 WHERE passengers.passport_number = (
  SELECT people.passport_number
  FROM people
  JOIN suspects ON people.id = suspects.person_id
GROUP BY suspects.person_id
ORDER BY COUNT(suspects.person_id) DESC
LIMIT 1
 );

-- CREATE table that stores person whome can be helpers
CREATE TABLE helpers (
  person_id INTEGER,
  FOREIGN KEY(person_id) REFERENCES people(id)
);

-- Clue 4: on phone call (receiver) -- for helper
-- add to helpers
INSERT INTO helpers (person_id)
SELECT  DISTINCT people.id
  FROM people
  JOIN phone_calls ON phone_calls.receiver = people.phone_number
 WHERE phone_calls.month = 7 AND phone_calls.day = 28 AND phone_calls.duration < 60
   AND phone_calls.caller = (
    SELECT people.phone_number
  FROM people
  JOIN suspects ON people.id = suspects.person_id
GROUP BY suspects.person_id
ORDER BY COUNT(suspects.person_id) DESC
LIMIT 1
   );

-- Clue 3: helper can be go with theif in same flight 
--         add to suspect 
INSERT INTO helpers (person_id) 
SELECT DISTINCT people.id
  FROM people
  JOIN passengers ON passengers.passport_number = people.passport_number
  JOIN flights ON flights.id = passengers.flight_id
  JOIN airports ON airports.id = flights.origin_airport_id
 WHERE flights.month = 7 AND flights.day = 29
   AND airports.city = 'Fiftyville'
   AND flights.id = (
    SELECT flight_id
      FROM passengers
     WHERE passport_number = (
        SELECT people.passport_number
          FROM people
          JOIN suspects ON people.id = suspects.person_id
        GROUP BY suspects.person_id
        ORDER BY COUNT(suspects.person_id) DESC
        LIMIT 1
     )
   )
   AND people.id IN (
    SELECT person_id
    FROM helpers
   );

-- To see helpers name
SELECT person_id, people.name
  FROM helpers
  JOIN people ON people.id = helpers.person_id;

-- most suspectable with highest probability (May be Thief helper)
SELECT people.name AS 'The ACCOMPLICE is'
  FROM helpers
  JOIN people ON people.id = helpers.person_id
GROUP BY person_id
ORDER BY COUNT(*) DESC;

-- DROP suspectes and helpers table
DROP TABLE suspects; 
DROP TABLE helpers; 