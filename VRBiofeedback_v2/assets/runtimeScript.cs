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

	public GameObject leftController;
	public GameObject rightController;

	public float leftMax;
	public float rightMax;

	public float dotHeight;
	public float dotWidth;

	public float targetHeight;
	public float targetWidth;

	public string pathToRightData;	//filepath to emg data for right muscles
	public string pathToLeftData;	//filepath to emg data for left muscles

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

		//Calculate left and right values as percentage of their max
		if (rightValue>rightMax) {
			rightMax = rightValue;
		}
		float rightPercent = rightValue / rightMax;


		if (leftValue>leftMax) {
			leftMax = leftValue;
		}
		float leftPercent = leftValue / leftMax;


		float height = (rightPercent + leftPercent) / 2;	//Get current height (0 - 1, aka 0 - 100%)
		float dotX = rightPercent / (rightPercent + leftPercent); 		//Get X value -> whether dot is closer to left, right, or middle (0 to 1)

		//adjust dot position
		moveDot (height, dotX, dotWidth, dotHeight);


		//ADJUST TARGET
		float targetPercentage = getTargetPercentage();
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
		targetObject.rectTransform.sizeDelta = new Vector2(width, height);
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

	float getLeftData(){
		StreamReader sr = new StreamReader (pathToLeftData);
		string leftDataString = sr.ReadToEnd ();
		float leftValue = (float)double.Parse (leftDataString);
		sr.Close ();
		return leftValue;
	}
	float getRightData(){
		StreamReader sr = new StreamReader (pathToRightData);
		string rightDataString = sr.ReadToEnd ();
		float rightValue = (float)double.Parse (rightDataString);
		sr.Close ();
		return rightValue;
	}
	float getTargetPercentage(){
		StreamReader sr = new StreamReader (pathToTargetPercentage);
		string targetPercentageString = sr.ReadToEnd ();
		sr.Close ();
		return (float)double.Parse (targetPercentageString) / 100;
	}
		
	void setMax(){
		leftMax = getLeftData ();
		rightMax = getRightData ();
	}
}
