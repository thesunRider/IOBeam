# Generated by Django 4.0.2 on 2022-02-27 10:38

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('home', '0004_device_device_data'),
    ]

    operations = [
        migrations.AlterField(
            model_name='device',
            name='device_uid',
            field=models.CharField(default='0', max_length=100, unique=True),
        ),
    ]
