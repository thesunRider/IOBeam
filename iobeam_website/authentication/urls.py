from django.urls import path
from . import views

urlpatterns = [
	path('login', views.login, name='login'),
	path('register', views.register, name='register'),
	path('password_reset', views.password_reset, name='password_reset'),
	path('register_user',views.register_user,name='register_user'),	
	path('login_user',views.login_user,name='login_user'),
	path('logout_user',views.logout_user,name='logout_user')
]