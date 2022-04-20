from django.db import models
from django.contrib.auth.models import User 
# Create your models here.

class user(models.Model):
	user_name = models.CharField( max_length = 100)
	user_email = models.CharField( max_length = 100)
	user_passhash = models.CharField( max_length = 100)
	user_uid = models.IntegerField( max_length = 7)

	def __str__(self):
		return self.user_name

	class Meta:  
		db_table = "user"  

