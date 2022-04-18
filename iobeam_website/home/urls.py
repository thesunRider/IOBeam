from django.urls import path

from . import views

urlpatterns = [
    path('', views.index, name='dashboard'),
    path('delete_model/',views.delete_model,name='delete_model')
]