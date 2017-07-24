using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEditor;
using System.IO;

public class OneBarUpdateScript : MonoBehaviour {

	public Text valueTextObject;		//displays current percent activation
	public Image fillContainerObject;	//background of red bar, used as reference for height
	public Image fillImageObject;		//red bar, moved and resized according to percent activation
	public Image targetNotch;			//white notch moved up and down
	public Text targetText;				//text displayed next to white notch indicating target percentage

	public string pathToEMGData;		//path to the file that contains percentage of activation

	public string pathToTargetPercentage;		//path to the file that contains target percentage

	// Use this for initialization
	void Start () {
	}
	
	// Update is called once per frame
	void Update () {

		setBarHeight();
		setCurrentValue();
		setTargetPercentage();
	}
	void setBarHeight(){
		//Read Current Activation Percent
		StreamReader sr = new StreamReader(pathToEMGData);
		string currentActivationString = sr.ReadToEnd();
		sr.Close();

		//get current activation as a decimal
		float scale = (float)double.Parse(currentActivationString) / 100;

		//calculate new size and position of red bar
		float containerHeight = fillContainerObject.rectTransform.rect.height;
		float fillImageHeight = containerHeight * scale;
		float centerDisplacement = (containerHeight - fillImageHeight) / 2;

		//move and resize image
		Vector3 newImagePosition = fillImageObject.rectTransform.localPosition;
		newImagePosition.y = fillContainerObject.rectTransform.localPosition.y - centerDisplacement;
		fillImageObject.rectTransform.localPosition = newImagePosition;
		fillImageObject.rectTransform.sizeDelta = new Vector2(fillContainerObject.rectTransform.rect.width, fillImageHeight);
	}
	void setCurrentValue(){
		//Read Current Activation Percent
		StreamReader sr = new StreamReader(pathToEMGData);
		string data = sr.ReadToEnd();
		sr.Close();

		//update value text
		valueTextObject.text = "Current Activation: " + data + "%";
	}
	void setTargetPercentage(){

		StreamReader sr = new StreamReader(pathToTargetPercentage);
		string data = sr.ReadToEnd();
		sr.Close();
		float targetPercentage = (float)double.Parse(data); 
		float percentage = targetPercentage / 100;

		float containerHeight = fillContainerObject.rectTransform.rect.height;
		float offset = (containerHeight * percentage) - (containerHeight / 2);

		Vector3 newPosition = targetNotch.rectTransform.localPosition;
		newPosition.y = fillContainerObject.rectTransform.localPosition.y + offset;
		targetNotch.rectTransform.localPosition = newPosition;

		targetText.text = targetPercentage + "%";
	}
}
