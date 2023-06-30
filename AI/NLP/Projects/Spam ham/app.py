from flask import Flask,render_template,url_for,request
import joblib 

vectorizer = joblib.load("vectorizer.pkl")
spamorham_model = joblib.load("spam_ham_model.pkl")

app = Flask(__name__)

@app.route('/')
def hello_World():
	return "Hello World"

@app.route('/spamorhaam', methods = ['GET', 'POST'])
def spamorham():
	message = request.args.get("message")
	vect_message = vectorizer.transform([message])
	result = spamorham_model.predict(vect_message)[0]
	return result

if __name__ == '__main__':
	app.run(debug=True)
	