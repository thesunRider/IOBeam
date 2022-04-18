
from django import forms
from django_ace import AceWidget

class EditorForm(forms.Form):
    script_name = forms.CharField(label='Script name', max_length=100)
    text = forms.CharField(label='Script', widget=AceWidget(mode='python', theme='twilight',width="100%",fontsize="18px",))