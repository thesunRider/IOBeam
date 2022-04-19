from django.urls import path

from . import views

urlpatterns = [
    path('connector_registerdevice',views.connector_registerdevice,name="connector_registerdevice"),
    path('connector_listdevices',views.connector_listdevices,name="connector_listdevices"),
    path('get_status',views.get_status,name="get_status"),
]