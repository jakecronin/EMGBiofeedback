using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEditor;
using System.IO;

public class runtimeScript : MonoBehaviour {

	public Text valueTextObject;
	public Image fillContainerObject;
	public Image fillImageObject;
	public Image targetNotch;
	public Text targetText;
	public GameObject leftController;
	public GameObject rightController;

	public float min;
	public float max;

	public string pathToEMGData;

	public float targetPercentage;


	//used to send haptic feedback
	private SteamVR_TrackedObject trackedObjectLeft;
	private SteamVR_TrackedObject trackedObjectRight;
	private SteamVR_Controller.Device device; 

	//used for delegate
	private SteamVR_TrackedController trackedControllerLeft;
	private SteamVR_TrackedController trackedControllerRight;



	// Use this for initialization
	void Start () {
		setUpControllers();
	}
	
	// Update is called once per frame
	void Update () {
		StreamReader sr = new StreamReader (pathToEMGData);
		string data = sr.ReadToEnd ();
		sr.Close ();

		//update value text
		valueTextObject.text = data;

		//get float value for data string
		float dataValue = (float) double.Parse (data);

		//update max if needed
		if (dataValue > max) {
			max = dataValue;
			Debug.Log ("updated max");
		}

		//slide fillImage to scaled percentage
		float scaledValue = ((dataValue / (max - min)) + min) * 100;
		slideView (scaledValue);

		setTargetPercentage ();

	}
	void setTargetPercentage(){
		float percentage = targetPercentage / 100;

		float containerHeight = fillImageObject.rectTransform.rect.height;
		float offset = (containerHeight * percentage) - (containerHeight / 2);

		Vector3 newPosition = targetNotch.rectTransform.localPosition;
		newPosition.y = fillContainerObject.rectTransform.localPosition.y + offset;
		targetNotch.rectTransform.localPosition = newPosition;

		targetText.text = targetPercentage + "%";
	}
	void slideView(float percentage){

		//get image displacement and height
		float containerHeight = fillContainerObject.rectTransform.rect.height;
		float fillImageHeight = containerHeight * percentage;
		float centerDisplacement = (containerHeight - fillImageHeight) / 2;

		//move and resize image
		Vector3 newImagePosition = fillImageObject.rectTransform.localPosition;
		newImagePosition.y = fillContainerObject.rectTransform.localPosition.y + centerDisplacement;
		fillImageObject.rectTransform.sizeDelta = new Vector2 (fillContainerObject.rectTransform.rect.width, fillImageHeight);
	}

	void setMax(){
		StreamReader sr = new StreamReader (pathToEMGData);
		string data = sr.ReadToEnd ();
		sr.Close ();
		float value = (float)double.Parse (data);
		max = value;
	}
	void setUpControllers(){
		//connect left trigger to trigger functions
		trackedControllerLeft = leftController.GetComponent<SteamVR_TrackedController> ();
		trackedControllerLeft.TriggerClicked += LeftTriggerPressed;
		trackedObjectLeft = leftController.GetComponent<SteamVR_TrackedObject> ();

		trackedControllerRight = rightController.GetComponent<SteamVR_TrackedController>();
		trackedControllerRight.TriggerClicked += RightTriggerPressed;
		trackedObjectRight = rightController.GetComponent<SteamVR_TrackedObject> ();
	}
	void LeftTriggerPressed(object sender, ClickedEventArgs e){
		device = SteamVR_Controller.Input ((int)trackedObjectLeft.index);
		device.TriggerHapticPulse (900);
		Debug.Log ("Left trigger pressed");
		setMax();
	}
	void RightTriggerPressed(object sender, ClickedEventArgs e){
		device = SteamVR_Controller.Input ((int)trackedObjectRight.index);
		device.TriggerHapticPulse (900);
		Debug.Log ("Right trigger pressed");
		setMax ();
	}

}
