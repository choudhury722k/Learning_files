import pickle
# Import all the packages you need for your model below
import numpy as np
import sys
import sklearn
# Import Flask for creating API
from flask import Flask, request

# Load the trained model from current directory
with open('./model.pkl', 'rb') as model_pkl:
    knn = pickle.load(model_pkl)

# Initialise a Flask app
app = Flask(__name__)

# Create an API endpoint
@app.route('/predict')
def predict_iris():
    # Read all necessary request parameters
    sl = request.args.get('sl')
    sw = request.args.get('sw')
    pl = request.args.get('pl')
    pw = request.args.get('pw')

    # Use the predict method of the model to 
    # get the prediction for unseen data
    unseen = np.array([[sl, sw, pl, pw]])
    result = knn.predict(unseen)
    
    # return the result back
    return 'Predicted result for observation ' + str(unseen) + ' is: ' + str(result)


if __name__ == '__main__':
    app.run()