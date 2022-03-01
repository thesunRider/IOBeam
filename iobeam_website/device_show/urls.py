from django.urls import path

from . import views

urlpatterns = [
    path('', views.device_show, name='device_show'),
]