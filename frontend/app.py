from flask import Flask, render_template, request
from subprocess import run, PIPE
from os import listdir

app = Flask(__name__)

@app.route('/', methods=['POST', 'GET'])
def index():
    # TODO: execute shell command and post the output
    if request.method == 'POST':
        print(request.form)
        file_name = request.form["file_to_decrypt"]
        answer1 = request.form["answer1"].strip()
        answer2 = request.form["answer2"].strip()
        answer3 = request.form["answer3"].strip()

        key = f"{answer1}{answer2}{answer3}"

        # call sillyCipher with args
        # Install sillyCipher util from https://github.com/fredlee590/SillyCipher
        # TODO: import as C stuff
        try:
            out = run(["sillyCipher", "-k", key, "-d", "-f", f"messages/{file_name}"], stdout=PIPE)
            print(out)
        except Exception as e:
            return f"Something bad happened: {e}"

        decrypted_str = out.stderr if out.stderr else out.stdout
        return render_template('display.html', message=decrypted_str.decode('utf-8').split('\n'))
    else:
        files = listdir('messages')
        return render_template('index.html', files=files)

if __name__ == "__main__":
    app.run(debug=True)
