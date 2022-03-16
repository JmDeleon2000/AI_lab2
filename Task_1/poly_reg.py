
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from sklearn import linear_model, metrics
from sklearn.linear_model import LinearRegression
from sklearn.preprocessing import PolynomialFeatures
from sklearn.model_selection import train_test_split
from sklearn.metrics import mean_squared_error, r2_score

class main(object):
    def __init__(self, csv_file) -> None:
        self.data = pd.read_csv(csv_file, encoding= 'unicode_escape')
        self.df = self.data

    def define_var(self):
        df = self.df

        # numero de la semana
        df['Week_Number'] = pd.to_datetime(df['Date'], dayfirst=True).dt.isocalendar().week
        
        # variables independientes -> store, date, holiday_flag, temperature, fuel_price, cpi, unemployment
        X = df.loc[:, ['Week_Number', 'Holiday_Flag', 'Temperature', 'Fuel_Price', 'CPI', 'Unemployment']]
        # variable dependiente -> weekly_sales
        y = df[['Weekly_Sales']]

        return X, y

    def train_test(self):
        df = self.df
        X, y = self.define_var()

        X_train, X_test,y_train, y_test = train_test_split(X, y, test_size = 0.3, train_size = 0.7)
        
        
        
        return X_train, X_test,y_train, y_test, 
    
    def poly_regression(self):
        X_train, X_test,y_train, y_test,  = self.train_test()
        X, y = self.define_var()

        poly = PolynomialFeatures(degree=5)

        Xtr_poly = poly.fit_transform(X_train)
        Xt_poly= poly.fit_transform(X_test)

        linear_reg = linear_model.LinearRegression()
        linear_reg.fit(Xtr_poly, y_train)

        # predicciones
        ytr_pred = linear_reg.predict(Xtr_poly)
        yt_pred = linear_reg.predict(Xt_poly)

        # xt, yt = zip(*sorted(zip(X_test['Unemployment'], yt_pred)))
        x2, y2 = zip(*sorted(zip(X_train['Fuel_Price'], ytr_pred)))

        # 'Holiday_Flag', 'Temperature', 'Fuel_Price', 'CPI', 'Unemployment'
        plt.scatter(X['Fuel_Price'], y, color='green', s=2)
        plt.plot(x2, y2, '-', color='pink', markersize=5)
 
        plt.xlabel('Fuel_Price')
        plt.ylabel('Weekly_Sales')
        

        # performance metricsss train
        rmse_train = np.sqrt(mean_squared_error(y_train, ytr_pred))
        r2_train = r2_score(y_train, ytr_pred)

        # performance metricsss testt
        rmse_test = np.sqrt(mean_squared_error(y_test, yt_pred))
        r2_test = r2_score(y_test, yt_pred)

        print("\nMétrica de desempeño Fuel_Price")
        print("-------------------------------------------")
        print("RMSE_train {}".format(rmse_train))
        print("R2_train {}".format(r2_train))
   
        print("-------------------------------------------")
        print("RMSE_test {}".format(rmse_test))
        print("R2_test {}".format(r2_test))
        print("\n")

        plt.show()




reader = main('../Walmart.csv')
reader.poly_regression()
