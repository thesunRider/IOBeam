from django.http import HttpResponse
from django.shortcuts import render, redirect
from django.contrib.auth.decorators import login_required
from django.contrib.auth import authenticate
from django.contrib.auth import login as log_in 
from django.contrib.auth import logout as log_out
from django.contrib.auth.models import User

from .forms import UserForm, UserRegistrationForm
# Create your views here.

def login(request):
	if request.user.is_authenticated:
		return redirect("dashboard") 

	return render(request, "authbase_login.template")

def register(request):
	if request.user.is_authenticated:
		return redirect("dashboard") 
	return render(request, "authbase_register.template")

def password_reset(request):
	if request.user.is_authenticated:
		return redirect("dashboard") 

	return render(request, "authbase_reset.template")

def register_user(request):
	# if this is a POST request we need to process the form data
	if request.method == 'POST':
		# create a form instance and populate it with data from the request:
		form = UserRegistrationForm(request.POST)
		# check whether it's valid:
		if form.is_valid():
			username = request.POST['username']
			password = request.POST['password']
			email = request.POST['email']
			newuser = User.objects.create_user(
						username=username,
						password=password,
						email=email
					)
			try:
				newuser.save()
				return redirect('login')
			except:
				return HttpResponse("Something went wrong.")

	return redirect("register")



def login_user(request):
	# if this is a POST request we need to process the form data
	if request.method == 'POST':
		email = request.POST['email']
		password =  request.POST['password']

		if 'rememberme' in request.POST: 
			remember = request.POST['rememberme']
		else:
			remember = "0"
		
		print(">USR mail:",email,"password:",password,"remember:",remember)

		user = authenticate(
				request, 
				username=email, 
				password=password
		)
		if user is None:
			return HttpResponse("Invalid credentials.")

		else:
			if remember == "1":
				request.session.set_expiry(0)

			log_in(request, user)
			return redirect('dashboard')

	return redirect("login")


@login_required
def logout_user(request):
	print("logout started")
	log_out(request)
	return redirect("login")