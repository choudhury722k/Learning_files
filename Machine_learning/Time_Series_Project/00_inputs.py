# 00 Inputs 

# Input file name with path
input_file_name = 'time_series_data.csv'

# Target class name
input_target_variable = 'value'

# Date column name
input_date_variable = 'date'

# Exogenous variable
input_exogenous_variable = 'month_no'

# Handle missing value
input_treat_missing_value = 'interp1d' # choose how to handle missing values from 'ffill','bfill' and 'interp1d'

# Box-cox transformation flag
input_transform_flag = 'No' # choose if you wish to transform the data - 'Yes' or 'No'

# Seasonality
input_seasonality = 12
input_order = (0, 1 , 2)
input_seasonal_order = (2, 1, 0, input_seasonality)

# Forecasting algorithm
input_ts_algo = 'auto_sarima' # choose the forecasting algorithm from 'auto_arima', 'auto_sarima', 'auto_sarimax', 'manual_sarima', 'simple_exponential_smoothing' and 'holt_winters'
