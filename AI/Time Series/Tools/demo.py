import streamlit as st
import pandas as pd
import numpy as np
from fbprophet import Prophet
from fbprophet.diagnostics import performance_metrics
from fbprophet.diagnostics import cross_validation
from fbprophet.plot import plot_cross_validation_metric
import base64

st.title('Time Series Forecasting Using Streamlit')

st.write("IMPORT DATA")
st.write("Import the time series csv file. It should have two columns labelled as 'ds' and 'y'.The 'ds' column should be of datetime format  by Pandas. The 'y' column must be numeric representing the measurement to be forecasted.")

data = st.file_uploader('Upload here',type='csv')

if data is not None:
    appdata = pd.read_csv(data)
    appdata['ds'] = pd.to_datetime(appdata['ds'],errors='coerce') 
    
    st.write(data)
    
    max_date = appdata['ds'].max()

st.write("SELECT FORECAST PERIOD")

periods_input = st.number_input('How many days forecast do you want?',
min_value = 1, max_value = 365)

if data is not None:
    obj = Prophet()
    obj.fit(appdata)

st.write("VISUALIZE FORECASTED DATA")
st.write("The following plot shows future predicted values. 'yhat' is the predicted value; upper and lower limits are 80% confidence intervals by default")

if data is not None:
    future = obj.make_future_dataframe(periods=periods_input)
    
    fcst = obj.predict(future)
    forecast = fcst[['ds', 'yhat', 'yhat_lower', 'yhat_upper']]

    forecast_filtered =  forecast[forecast['ds'] > max_date]    
    st.write(forecast_filtered)

    
    st.write("The next visual shows the actual (black dots) and predicted (blue line) values over time.")    

    figure1 = obj.plot(fcst)
    st.write(figure1)
 
    
    st.write("The next few visuals show a high level trend of predicted values, day of week trends, and yearly trends (if dataset covers multiple years). The blue shaded area represents upper and lower confidence intervals.")
      

    figure2 = obj.plot_components(fcst)
    st.write(figure2)
