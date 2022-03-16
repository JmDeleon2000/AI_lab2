import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn import svm, datasets
from sklearn.model_selection import train_test_split, GridSearchCV
from sklearn.metrics import  classification_report



data = pd.read_csv("dataset.csv")

dataset = pd.DataFrame(data)
dataset['name'] = dataset['name'].map({'grapefruit': 0, 'orange': 1}, na_action=None)

y = dataset[dataset.columns[0]].values
x = dataset[dataset.columns[1:3]].values


#Preparacion de datos y meshgrid para graficar
min_x = x[:, 0].min() - 1 
min_y = x[:, 1].min() - 1
max_x = x[:, 0].max() + 1
max_y = x[:, 1].max() + 1


b = (max_x / min_x)/100
xx, yy = np.meshgrid(np.arange(min_x, max_x, b),np.arange(min_y, max_y, b))

#Modelo SVC linear
SVM_Model_Lineal = svm.SVC(kernel='linear', C=1)
SVM_Model_Lineal.fit(x, y)

plt.subplot(1, 1, 1)
Z = SVM_Model_Lineal.predict(np.c_[xx.ravel(), yy.ravel()])
Z = Z.reshape(xx.shape) 
plt.contourf(xx, yy, Z, cmap=plt.cm.Paired, alpha=0.8)

plt.scatter(x[:, 0], x[:, 1], c=y, cmap=plt.cm.Paired)
plt.xlabel('Diametro')
plt.ylabel('Peso')
plt.xlim(xx.min(), xx.max())
plt.title('SVM con Kernel Linear')
plt.show()


#Modelo SVC RBF
SVM_Model_RBF = svm.SVC(kernel='rbf', C=1, gamma=10)
SVM_Model_RBF.fit(x, y)

plt.subplot(1, 1, 1)
K = SVM_Model_RBF.predict(np.c_[xx.ravel(), yy.ravel()])
K = K.reshape(xx.shape) 
plt.contourf(xx, yy, K, cmap=plt.cm.Paired, alpha=0.8)

plt.scatter(x[:, 0], x[:, 1], c=y, cmap=plt.cm.Paired)
plt.xlabel('Diametro')
plt.ylabel('Peso')
plt.xlim(xx.min(), xx.max())
plt.title('SVM con Kernel RBF')
plt.show()


# Designacion de sets de para entrenamiento y prueba
X_train, X_test, y_train, y_test = train_test_split(dataset[dataset.columns[:]].values, 
                                                    dataset[dataset.columns[0]].values, 
                                                    test_size=0.7, random_state=123)

# Entrenamiento de modelo con el set de train en modelo linear
model = svm.SVC(kernel='linear', C=1, gamma = 10)
model.fit(X_train, y_train)


# Entrenamiento de modelo con el set de train en modelo rbf
model2 = svm.SVC(kernel='rbf', C=1, gamma = 10)
model2.fit(X_train, y_train)


# Impresion de prediccion linel 
predictions = model.predict(X_test)
print("Reporte de clasificacion kernel lineal: \n",classification_report(y_test, predictions))

predictions2 = model2.predict(X_test)
print("Reporte de clasificacion kernel rbf: \n",classification_report(y_test, predictions2))

# Definiendo rango de parametros para kernel Linear y RBF 
param_grid = {'C': [0.1, 1, 10, 100, 1000],
              'gamma': [1, 0.1, 0.01, 0.001, 0.0001],
              'kernel': [ 'rbf']}

param_grid2 = {'C': [1, 10, 100, 1000], 'kernel': ['linear']}

# Reajuste con los mejores parametros encontrados lineal
grid = GridSearchCV(svm.SVC(), param_grid2, refit=True, cv=3, verbose=3)

grid2 = GridSearchCV(svm.SVC(), param_grid, refit=True, cv=3, verbose=3)


# Fit para grid search 
grid.fit(X_train, y_train)

grid2.fit(X_train, y_train)

# Impresion de mejor parametro despues de tuning
print("Los mejores parametros para kernel lineal son: ", grid.best_params_)

print("Los mejores parametros para kernel rbf son: ", grid2.best_params_)



# Impresion de modelo actualizado despues de tuning 
print("Modelo con kernel linear: \n",grid.best_estimator_)
grid_predictions = grid.predict(X_test)

print("Modelo con kernel rbf: \n",grid2.best_estimator_)
grid_predictions2 = grid2.predict(X_test)


# print classification report
print("Reporte de clasificacion kernel lineal: \n",classification_report(y_test, grid_predictions))

print("Reporte de clasificacion kernel rbf: \n",classification_report(y_test, grid_predictions2))

# Nuevo Modelo SVC linear
SVM_Model_Lineal2 = svm.SVC(kernel=grid.best_params_['kernel'], C=grid.best_params_['C'])
SVM_Model_Lineal2.fit(x, y)

plt.subplot(1, 1, 1)
M = SVM_Model_Lineal2.predict(np.c_[xx.ravel(), yy.ravel()])
M = M.reshape(xx.shape) 
plt.contourf(xx, yy, M, cmap=plt.cm.Paired, alpha=0.8)

plt.scatter(x[:, 0], x[:, 1], c=y, cmap=plt.cm.Paired)
plt.xlabel('Diametro')
plt.ylabel('Peso')
plt.xlim(xx.min(), xx.max())
plt.title('Nuevo SVM con Kernel Linear')
plt.show()


# Nuevo Modelo SVC RBF
SVM_Model_RBF2 = svm.SVC(kernel=grid2.best_params_['kernel'], C=grid2.best_params_['C'], gamma=grid2.best_params_['gamma'])
SVM_Model_RBF2.fit(x, y)

plt.subplot(1, 1, 1)
Q = SVM_Model_RBF2.predict(np.c_[xx.ravel(), yy.ravel()])
Q = Q.reshape(xx.shape) 
plt.contourf(xx, yy, Q, cmap=plt.cm.Paired, alpha=0.8)

plt.scatter(x[:, 0], x[:, 1], c=y, cmap=plt.cm.Paired)
plt.xlabel('Diametro')
plt.ylabel('Peso')
plt.xlim(xx.min(), xx.max())
plt.title('Nuevo SVM con Kernel RBF')
plt.show()

