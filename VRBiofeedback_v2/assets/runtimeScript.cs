using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEditor;
using System.IO;

public class runtimeScript : MonoBehaviour {

	public Text valueTextObject;		//test to make sure data is being read in

	public Image graphObject;		//background graph
	public Image targetObject;		//target to hit
	public Image dotObject;			//moving dot

	public GameObject leftController;	//VR Controller
	public GameObject rightController;	//VR Controller

	//public float leftMax; unnecessary, scaled in labview
	//public float rightMax; unnecessary, scaled in labview

	public float dotHeight;
	public float dotWidth;

	//public float targetHeight; unnecessary, read from labview
	//public float targetWidth;	 unnecessary, read from labview

	public string pathToRightData;	//filepath to emg data for right muscles
	public string pathToLeftData;   //filepath to emg data for left muscles

	public string pathToLRErrorData;
	public string pathToAmplitudeErrorData;

	public string pathToTargetPercentage;	//filepath to number that represents target activation


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

		//Read Left and Right Values
		float rightValue = getRightData();
		float leftValue = getLeftData();

		//Don't need to scale anymore because values are percentages
		//if (rightValue>rightMax) {
		//	rightMax = rightValue;
		//}
		//float rightPercent = rightValue / rightMax;


		//if (leftValue>leftMax) {
		//	leftMax = leftValue;
		//}
		//float leftPercent = leftValue / leftMax;


		float height = (rightValue + leftValue) / 2;	//Get current height (0 - 1, aka 0 - 100%)
		float dotX = rightValue / (rightValue + leftValue); 		//Get X value -> whether dot is closer to left, right, or middle (0 to 1)

		//adjust dot position
		moveDot (height, dotX, dotWidth, dotHeight);


		//ADJUST TARGET
		float targetPercentage = getTargetPercentage();
		float targetWidth = getLRError() * 2;
		float targetHeight = getAmplitudeError() * 2;
		float mid = (float)0.5;
		moveTarget (targetPercentage, mid, targetWidth, targetHeight);


	}

	void moveDot(float yPercent, float xPercent, float width, float height){
		//get image displacement and height
		float graphHeight = graphObject.rectTransform.rect.height;
		float graphWidth = graphObject.rectTransform.rect.width;

		//move dot
		Vector3 newDotPosition = dotObject.rectTransform.localPosition;
		newDotPosition.y = graphObject.rectTransform.localPosition.y - (graphHeight / 2) + (graphHeight * yPercent);
		newDotPosition.x = graphObject.rectTransform.localPosition.x - (graphWidth / 2) + (graphWidth * xPercent);
		dotObject.rectTransform.localPosition = newDotPosition;

		//resize dot
		dotObject.rectTransform.sizeDelta = new Vector2(width, height);
	}

	void moveTarget(float yPercent, float xPercent, float width, float height){
		//get image displacement and height
		float graphHeight = graphObject.rectTransform.rect.height;
		float graphWidth = graphObject.rectTransform.rect.width;

		//move and resize image
		Vector3 newTargetPosition = targetObject.rectTransform.localPosition;
		newTargetPosition.y = graphObject.rectTransform.localPosition.y - (graphHeight / 2) + (graphHeight * yPercent);
		newTargetPosition.x = graphObject.rectTransform.localPosition.x - (graphWidth / 2) + (graphWidth * xPercent);
		targetObject.rectTransform.localPosition = newTargetPosition;

		//update target size
		float targetHeight = graphHeight * height;
		float targetWidth = graphWidth * width;

		targetObject.rectTransform.sizeDelta = new Vector2(targetWidth, targetHeight);
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
		//setMax();
	}
	void RightTriggerPressed(object sender, ClickedEventArgs e){
		device = SteamVR_Controller.Input ((int)trackedObjectRight.index);
		device.TriggerHapticPulse (900);
		Debug.Log ("Right trigger pressed");
		//setMax ();
	}

	float getLeftData(){
		StreamReader sr = new StreamReader (pathToLeftData);
		string leftDataString = sr.ReadToEnd ();
		float leftValue = (float)double.Parse (leftDataString) / 100;
		sr.Close ();
		return leftValue;
	}
	float getRightData(){
		StreamReader sr = new StreamReader (pathToRightData);
		string rightDataString = sr.ReadToEnd ();
		float rightValue = (float)double.Parse (rightDataString) / 100;
		sr.Close ();
		return rightValue;
	}
	float getTargetPercentage(){
		StreamReader sr = new StreamReader (pathToTargetPercentage);
		string targetPercentageString = sr.ReadToEnd ();
		sr.Close ();
		return (float)double.Parse (targetPercentageString) / 100;
	}
	float getLRError()
	{
		StreamReader sr = new StreamReader(pathToLRErrorData);
		string LRErrorString = sr.ReadToEnd();
		sr.Close();
		return (float)double.Parse(LRErrorString) / 100;
	}
	float getAmplitudeError()
	{
		StreamReader sr = new StreamReader(pathToAmplitudeErrorData);
		string amplitudeErrorString = sr.ReadToEnd();
		sr.Close();
		return (float)double.Parse(amplitudeErrorString) / 100;
	}
		
	//void setMax(){	No Longer Necessary as values are turned into percentages in LabView
	//	leftMax = getLeftData ();
	//	rightMax = getRightData ();
	//}
}
