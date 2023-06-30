# 09 Box-Cox Transformation

# Box - Cox Transform
if input_transform_flag == 'Yes':
    df[input_target_variable], lam = boxcox(df[input_target_variable])
else:
    print("No transformation")
