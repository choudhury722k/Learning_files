# 14 Save the final model

# save the model to disk
filename = 'final_model.sav'
pickle.dump(model, open(filename, 'wb'))

# load the model from disk
loaded_model = pickle.load(open(filename, 'rb'))
loaded_model
