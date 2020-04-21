from flask import Flask, render_template, redirect, request, url_for
from flask_sqlalchemy import SQLAlchemy
from subprocess import run, PIPE
from os import urandom, path
from random import randint
from tempfile import TemporaryDirectory
from datetime import datetime
import argparse

app = Flask(__name__)
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///sillyCipher.db'
db = SQLAlchemy(app)


# TODO: create database of key questions and ciphertext: Quizzes
class Quiz(db.Model):
    qid = db.Column(db.Integer, primary_key=True)
    questions = db.Column(db.String(2048), nullable=False)
    message = db.Column(db.String(10240), nullable=False)  # 10 kB
    date_created = db.Column(db.DateTime, default=datetime.utcnow)

    def __repr__(self):
        return '<Quiz %r>' % self.qid

@app.route('/display/<int:quiz_id>', methods=['GET', 'POST'])
def display(quiz_id):
    answers = request.form.values()
    key = ""
    for answer in answers:
        key += answer.replace(" ", "")
    quiz = Quiz.query.get_or_404(quiz_id)
    with TemporaryDirectory() as tempdirname:
        tempfilename = path.join(tempdirname, f"{quiz_id}.txt")
        with open(tempfilename, 'w') as f:
            f.write(quiz.message)
        try:
            out = run(["sillyCipher", "-k", key, "-d", "-f", tempfilename], stdout=PIPE)
        except Exception as e:
            return f"Something bad happened: {e}"

    decrypted_str = out.stderr if out.stderr else out.stdout
    return render_template('display.html', message=decrypted_str.decode('utf-8').split('\n'), quiz_id=quiz_id)

@app.route('/quiz/<int:quiz_id>')
def quiz(quiz_id):
    quiz = Quiz.query.get_or_404(quiz_id)
    return render_template('quiz.html', questions=quiz.questions.split('\n'), quiz_id=quiz_id)

@app.route('/', methods=['POST', 'GET'])
def index():
    if request.method == 'POST':  # add new quiz to database
        button_clicked = request.form["button"]
        if button_clicked in ["Go To Quiz", "Delete Quiz"]:
            quiz_id = request.form["quiz_id"]
            quiz = Quiz.query.get_or_404(quiz_id)

        if button_clicked == "Go To Quiz":
            return redirect(f'/quiz/{quiz_id}')
        elif button_clicked == "Delete Quiz":
            try:
                db.session.delete(quiz)
                db.session.commit()
                return redirect('/')
            except:
                return "Issue deleting quiz %d" % quiz_id
        elif button_clicked == "Add Quiz":
            while True:
                new_qid = randint(1000, 9999)

                if Quiz.query.filter_by(qid=new_qid).first() is None:
                    break

            new_questions_file = request.files["questions_file"]
            new_encrypted_file = request.files["encrypted_file"]

            if new_questions_file and new_encrypted_file:
                new_questions_str = new_questions_file.stream.read().decode('utf-8')[:-1]
                new_encrypted_str = new_encrypted_file.stream.read().decode('utf-8')

                new_quiz = Quiz(qid=new_qid, questions=new_questions_str, message=new_encrypted_str)
            try:
                db.session.add(new_quiz)
                db.session.commit()
            except:
                return "Issue occurred adding quiz"

            return redirect('/')
        else:
            return "UNKNOWN BUTTON"
    else:
        quizzes = Quiz.query.order_by(Quiz.qid).all()
        last_created = None if not len(quizzes) else Quiz.query.order_by(Quiz.date_created)[-1].qid
        return render_template('index.html', quizzes=quizzes, last_created=last_created)

def parse_args():
    parser = argparse.ArgumentParser(description="Silly Cipher front end web app")

    parser.add_argument("-c", "--certs", action='store_true', help="Run with local certs")

    return parser.parse_args()

if __name__ == "__main__":
    args = parse_args()

    db.create_all()
    ssl_context = ('cert.pem', 'key.pem') if args.certs else None
    app.run(debug=True, ssl_context=ssl_context)
