# Generated by Django 4.0.2 on 2022-02-27 10:51

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('home', '0005_alter_device_device_uid'),
    ]

    operations = [
        migrations.AlterField(
            model_name='device',
            name='device_uid',
            field=models.CharField(default='0', error_messages={'unique': 'You cant-re register the same device.'}, max_length=100, unique=True),
        ),
    ]
