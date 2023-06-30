# 04 Handle Missing Values

# Select how you wish to treat missing values according to the input provided
if input_treat_missing_value == 'ffill':
    
    # forward fill missing values
    df = df.ffill()

elif input_treat_missing_value == 'bfill': 
    
    # backward fill missing values
    df = df.bfill()
    
elif input_treat_missing_value == 'interp1d':
    
    ## linear Interpolation
    df['rownum'] = np.arange(df.shape[0])
    df_nona = df.dropna(subset = ['value'])
    f = interp1d(df_nona['rownum'], df_nona['value'])
    df[input_target_variable] = f(df['rownum'])
    df = df.drop('rownum', axis = 1)
