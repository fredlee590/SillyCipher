# Silly Cipher Web App

The Silly Cipher Web App is the online face of the sillyCipher utility. Users are presented with a series of questions and a choice of a file to decrypt. These answers are used to generate a key with which to decrypt the selected file. In this way, the web app administrator can share select files designed for consumption by authorized users only and the programmer can learn and exercise web application concepts.

# Set Up

The web app is built entirely in Python 3.6 and relies on having the `sillyCipher` utility in the server's path. Install Python3.6 in the usual way and compile and install `sillyCipher` according to the README in the parent directory.

The project also relies on the Flask Python library. To install this on the server, simply execute the following commands. If it is desireable to run this web app on the native environment, the steps that set up the virtual environment may, of course, be omitted.

```
pip3 install virtualenv
virtualenv env
source env/bin/activate

pip3 install -r requirements.txt
```

You are now ready to start the web app.

# How To

## Add Messages and Questions

To add a message to decrypt, simply
1. Write your message to a file
1. Encrypt it with the `sillyCipher` utility with a custom keyword. Save to a file.
1. Write a newline-separated file of questions. For best results, the answers to these questions should make up your keyword. Save this file.
1. Navigate to the SillyCipher web app in a browser.
1. Upload the Questions and Message files to their corresponding elements
1. Click the Add Quiz button
1. Take note of the randomly assigned Quiz ID of the most recently added quiz at the bottom of the page

You are now ready to answer its questions!

## Start

Start the web app, through the flask utility.

```
flask --app app.py run
```

## View

Open a browser to localhost, the server's IP address, or its domain name, specifying the default port of 5000. From there, have fun answering questions or send the URL to your friends and have them guess as well, with the prize being your decrypted messages!

As an example, enter the strings `ka`, `ta`, and `na` for questions 1, 2, and 3 and see the decrypted sample message.
