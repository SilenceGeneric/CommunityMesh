from flask import Flask, render_template, request, redirect, url_for, send_from_directory
import os

# Create a Flask application
app = Flask(__name__)

# Define the directory for the library
LIBRARY_DIR = 'library'

# Route for the home page
@app.route('/')
def index():
    files = os.listdir(LIBRARY_DIR)
    return render_template('index.html', files=files)

# Route for serving library files
@app.route('/library/<path:filename>')
def send_file(filename):
    return send_from_directory(LIBRARY_DIR, filename)

# Route for uploading files
@app.route('/upload', methods=['POST'])
def upload_file():
    if 'file' not in request.files:
        return redirect(request.url)
    file = request.files['file']
    if file.filename == '':
        return redirect(request.url)
    file.save(os.path.join(LIBRARY_DIR, file.filename))
    return redirect(url_for('index'))

# Route for deleting files
@app.route('/delete/<path:filename>')
def delete_file(filename):
    os.remove(os.path.join(LIBRARY_DIR, filename))
    return redirect(url_for('index'))

if __name__ == '__main__':
    if not os.path.exists(LIBRARY_DIR):
        os.makedirs(LIBRARY_DIR)
    app.run(host='0.0.0.0', port=5000)
