-- 11. Titles of the five highest rated movies (in order) that Chadwick Boseman starred in, starting with the highest rated

-- WITH JOINs
SELECT DISTINCT title FROM movies
JOIN ratings ON movies.id = ratings.movie_id
JOIN stars ON ratings.movie_id = stars.movie_id
JOIN people ON stars.person_id = people.id
WHERE people.name = 'Chadwick Boseman' 
ORDER BY ratings.rating DESC
LIMIT 5;

-- WITH nested SELECTs
SELECT title
FROM movies
WHERE id IN (
    SELECT movie_id FROM stars
    WHERE person_id = (
        SELECT id FROM people
        WHERE name = 'Chadwick Boseman' 
    ) 
)
ORDER BY (
    SELECT rating
    FROM ratings
    WHERE ratings.movie_id = movies.id
) DESC
LIMIT 5;