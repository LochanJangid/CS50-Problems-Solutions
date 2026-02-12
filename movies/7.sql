-- 7. All movies and ratings from 2010, in decreasing order by rating (alphabetical for those with same rating)
SELECT title, rating, year
FROM movies
JOIN ratings ON movies.id = ratings.movie_id
WHERE movies.year = 2010 AND ratings.rating IS NOT NULL
ORDER BY rating DESC, movies.title;