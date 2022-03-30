
from django import forms
from django_ace import AceWidget

class EditorForm(forms.Form):
    text = forms.CharField(widget=AceWidget(mode='python', theme='twilight',width="100%",fontsize="18px",))
