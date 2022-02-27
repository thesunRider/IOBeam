from django.urls import path

from . import views

urlpatterns = [
    path('', views.index, name='dashboard'),
    path('connector_registerdevice',views.connector_registerdevice,name="connector_registerdevice")
]