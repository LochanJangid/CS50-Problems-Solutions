from flask import Flask, flash, jsonify, redirect, render_template, request, session
import sqlite3

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":
        conn = sqlite3.connect('birthdays.db')
        cursor = conn.cursor()

        name = request.form.get("name")
        month = request.form.get("month")
        day = request.form.get("day")

        if not name or not month or not day:
            return redirect('/')
        
        try:
            month = int(month)
            day = int(day)
        except ValueError:
            print('month and day should be int')
            return redirect('/')
        
        if not name.isalpha():
            print('name can not be non alphabetically')
            return redirect('/')
        
        if month < 1 or month > 12:
            print('month is not valid')
            return redirect('/')
        if day < 1 or day > 31:
            print('day is not valid')
            return redirect('/')

        print(name, month, day)

        cursor.execute('INSERT INTO birthdays(name, month, day) VALUES (?, ?, ?)', (name, month, day))
        conn.commit()

        conn.close()
        return redirect("/")

    else:
        conn = sqlite3.connect('birthdays.db')
        conn.row_factory = sqlite3.Row
        cursor = conn.cursor()

        cursor.execute('SELECT * FROM birthdays')
        birthdays = cursor.fetchall()
        print(birthdays)

        conn.close()
        return render_template("index.html", birthdays=birthdays)


