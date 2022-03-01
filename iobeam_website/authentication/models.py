from django.db import models
from django.contrib.auth.models import User 

class Employee(models.Model):
    user = models.OneToOneField(User, on_delete=models.CASCADE)
    department = models.CharField(max_length=100)


# Create your models here.

class user(models.Model):
	user_name = models.CharField( max_length = 100)
	user_email = models.CharField( max_length = 100)
	user_passhash = models.CharField( max_length = 100)

	def __str__(self):
		return self.user_name

	class Meta:  
		db_table = "user"  

