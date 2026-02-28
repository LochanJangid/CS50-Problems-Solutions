import os
import sqlite3
from datetime import datetime
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks""" 
    # connect to db
    with sqlite3.connect('finance.db') as conn:
        conn.row_factory = sqlite3.Row
        cursor = conn.cursor()
        # get user cash 
        cursor.execute('SELECT * FROM users where id =  ?', (session["user_id"], ))
        user_detail = cursor.fetchone()

        if not user_detail:
            return apology('INVALID USER')

        # get stocks data from share_transactions table
        cursor.execute('SELECT DISTINCT symbol, SUM(shares) AS shares FROM share_transactions WHERE user_id = ? GROUP BY symbol HAVING SUM(shares) > 0', (session["user_id"] ,))
        stock_data  = cursor.fetchall()

    stock_data  = [dict(row) for row in stock_data]
    total_val = 0
    for row in stock_data :
        stock_quote = lookup(row["symbol"]) # check current price
        row["price"] = usd(stock_quote["price"]) # insert into dict current price
        total = stock_quote["price"]*row["shares"]
        row["total"] = usd(total)
        total_val += total
    
    total_val += user_detail["cash"]

    return render_template("index.html", stock_data=stock_data, cash=usd(user_detail["cash"]), total_val=usd(total_val))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")


        if not symbol:
            return apology('MISSING SYMBOL')
        if not shares:
            return apology('MISSING SHARES')
        
        stock_info = lookup(symbol)
        # check shares are in integer or not OR NEGATIVE OR ZERO
        
        try:
            shares = int(shares)
        except ValueError:
            return apology('INVALID SHARES')

        if not shares > 0:
            return apology('INVALID SHARES')

        if not stock_info:
            return apology("INVALID SYMBOL")

        # connect to finance.db
        with sqlite3.connect('finance.db') as conn:
            conn.row_factory = sqlite3.Row
            cursor = conn.cursor()

            cursor.execute('SELECT * FROM users WHERE id = ?', (session["user_id"],))
            user_info = cursor.fetchone()

            if not user_info:
                return apology('INVALID USER')

            # check user can afford shares or not
            if stock_info["price"]*shares > user_info["cash"]:
                return apology("CAN'T AFFORD")

            # Create a table
            # CREATE TABLE share_transactions (share_id INTEGER PRIMARY KEY, user_id INTEGER NOT NULL, symbol TEXT NOT NULL, shares INTEGER NOT NULL, price INTEGER NOT NULL, transacted DATETIME NOT NULL, FOREIGN KEY (user_id) REFERENCES users(id));
            now = datetime.now()    
            now = now.strftime("%Y-%m-%d %H:%M:%S")
            # insert into share_transactions table
            cursor.execute('INSERT INTO share_transactions (user_id, symbol, shares, price, transacted) VALUES (?, ?, ?, ?, ?)', (session["user_id"],  symbol, shares, stock_info["price"], now))
            # update user cash
            left_cash = user_info["cash"] - stock_info["price"]*shares
            cursor.execute('UPDATE users SET cash = ? WHERE id = ?', ( left_cash, session["user_id"])) 

        flash("Bought!")
        return redirect('/')

    return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    # connect to db
    with sqlite3.connect('finance.db') as conn:
        conn.row_factory = sqlite3.Row
        cursor = conn.cursor()
        # get stocks data from share_transactions table
        cursor.execute('SELECT * FROM share_transactions WHERE user_id = ?', (session["user_id"] ,))
        stock_data  = cursor.fetchall()

    return render_template("history.html", stock_data=stock_data)



@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)
        
        with sqlite3.connect('finance.db') as conn:
            conn.row_factory = sqlite3.Row
            cursor = conn.cursor()

            cursor.execute("SELECT * FROM users WHERE username = ?", (request.form.get("username"),))
            rows = cursor.fetchall()

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    # if quote submitted
    if request.method == "POST":
            stock_info = lookup(request.form.get('symbol'))
            if not stock_info:
                return apology('INVALID SYMBOL')
            
            stock_name = stock_info["name"]
            stock_symbol = stock_info["symbol"]
            stock_price = usd(stock_info["price"])
            
            return render_template("quote.html", stock_name=stock_name, stock_symbol=stock_symbol, stock_price=stock_price)

    return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirm_password = request.form.get("confirm_password")
        
        if not username:
            return apology("must provide username", 403)
        if not password:
            return apology("must provide password", 403)
        if not confirm_password:
            return apology("must provide confirm password", 403)
        
        if password != confirm_password:
            return apology("confirm password dosen't match", 403)

        with sqlite3.connect('finance.db') as conn:
            conn.row_factory = sqlite3.Row
            cursor = conn.cursor()
            cursor.execute('SELECT * FROM users WHERE username = ?', (username,))
            row = cursor.fetchone()

            if row is not None:
                return apology("this username exist", 403)

            hash_password = generate_password_hash(password)
            # Insert into table users
            cursor.execute("INSERT INTO users (username, hash) VALUES (?, ?)", (username, hash_password))   

        session["user_id"] = cursor.lastrowid
        return redirect('/')

    return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    
    with sqlite3.connect('finance.db') as conn:
        conn.row_factory = sqlite3.Row
        cursor = conn.cursor()

        cursor.execute('SELECT DISTINCT symbol FROM share_transactions WHERE user_id = ?', (session["user_id"], ))
        owned_symbols = cursor.fetchall()

        # if sell
        if request.method == "POST":
            symbol = request.form.get("symbol")
            shares   = request.form.get("shares")
    
            # Validations
            if not symbol:
                return apology('MISSING SYMBOL')
            if not shares:
                return apology('MISSING SHARES')
            
            try:
                shares = int(shares)
            except ValueError:
                return apology('INVALID SHARES')
            if not shares > 0:
                return apology('INVALID SHARES')
            
            cursor.execute('SELECT SUM(shares) AS shares FROM share_transactions WHERE symbol = ? AND user_id = ?', (symbol, session["user_id"]))
            owned_shares = cursor.fetchone()

            if owned_shares["shares"] is None or owned_shares["shares"] <= 0:
                return apology('INVALID SHARES')

            if shares > owned_shares["shares"]:
                return apology('TOO MANY SHARES')

            # EVERYTHING IS OK - SELL IT
            stock_info = lookup(symbol) # sell at current price
            now = datetime.now()
            now = now.strftime("%Y-%m-%d %H:%M:%S")
            # sell shares will be in (-)
            cursor.execute('INSERT INTO share_transactions (user_id, symbol, shares, price, transacted) VALUES (?, ?, ?, ?, ?)', (session["user_id"],  symbol, - shares, stock_info["price"], now))
            # add MONEY INTO CASH
            cursor.execute('UPDATE users SET cash = cash + ? WHERE id = ?', (stock_info["price"]*shares, session["user_id"]))   

            flash("Sold!")
            return redirect('/')

        return render_template("sell.html", owned_symbols=owned_symbols)


# personal touch :)
@app.route("/add_cash", methods=["GET", "POST"])
@login_required
def add_cash():
    """Add more money for cash"""
    if request.method == "POST":
        amount = request.form.get("amount")

        try: 
            amount = int(amount)
        except ValueError:
            return apology('INVALID AMOUNT')
        
        if amount <= 0:
            return apology('INVALID AMOUNT')
        
        with sqlite3.connect('finance.db') as conn:
            cursor = conn.cursor()
            cursor.execute('UPDATE users SET cash = cash + ? WHERE id = ?', (amount, session["user_id"]))
        
        flash("Added!")
        return redirect('/')
    return render_template("add_cash.html")
    