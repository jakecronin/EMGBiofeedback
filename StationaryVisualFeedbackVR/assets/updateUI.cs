using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEditor;
using System.IO;


public class updateUI : MonoBehaviour {
	public float min;	//current minimum (set by editor)
	public float max;	//current maximum (initial set by editor, updated by script)

	public int targetPercentage;
	public string targetPercentageText;

	public GameObject currentForceLabel;	//label that displays the current force
	public GameObject targetPercentageLabel;	//size set by target percentage

	public string path;
	public RectTransform transform;

	//Controllers
	public GameObject controllerLeft;
	public GameObject controllerRight;
		//used for controller haptic feedback
	private SteamVR_TrackedObject trackedObjectLeft;
	private SteamVR_TrackedObject trackedObjectRight;
	private SteamVR_Controller.Device device;
		//used for controller delegate (button presses)
	private SteamVR_TrackedController trackedControllerLeft;
	private SteamVR_TrackedController trackedControllerRight;


	//Strings that are modified during runtime, copy inputs at Start
	private Text targetTextToModify;
	private Text currentForceTextToModify;


	// Use this for initialization
	void Start () {

		targetTextToModify = targetPercentageLabel.GetComponent<Text> ();
		if (targetPercentageText != null) {
			targetTextToModify = targetPercentageText;
		}
	
		currentForceTextToModify = currentForceLabel.GetComponent<Text> ();

		transform = GetComponent<RectTransform> ();

		//Set Delegate, trigger clicks cause triggerPressed function
		trackedControllerLeft = controllerLeft.GetComponent<SteamVR_TrackedController> ();
		trackedControllerLeft.TriggerClicked += LeftTriggerPressed;

		trackedControllerRight = controllerLeft.GetComponent<SteamVR_TrackedController> ();
		trackedControllerRight.TriggerClicked += RightTriggerPressed;

		//set delegates
		trackedObjectLeft = controllerLeft.GetComponent<SteamVR_TrackedObject> ();
		trackedObjectRight = controllerRight.GetComponent<SteamVR_TrackedObject> ();
	}

	void LeftTriggerPressed(object sender, ClickedEventArgs e){
		updateMax ();

		device = SteamVR_Controller.Input((int)trackedObjectLeft.index);
		device.TriggerHapticPulse (900);
		Debug.Log ("Left trigger pressed");
	}
	void RightTriggerPressed(object sender, ClickedEventArgs e){
		updateMax ();

		device = SteamVR_Controller.Input((int)trackedObjectRight.index);
		device.TriggerHapticPulse (900);
		Debug.Log ("Right trigger pressed");
	}
	void updateMax(){
		StreamReader sr = new StreamReader (path);
		string data = sr.ReadToEnd ();
		sr.Close ();
		float value = (float)double.Parse (data);
		max = value;
	}

	// Update is called once per frame
	void Update () {
		//read data
		StreamReader sr = new StreamReader (path);
		string data = sr.ReadToEnd ();
		sr.Close ();
		//update string
		currentForceTextToModify.text = "Percent Force: " + data;

		//convert to double and send to slideView
		float value = (float) double.Parse (data);
		if (value > max) {
			max = value;
			Debug.Log ("updated max");
		}
		//scale data
		value = ((value / (max - min)) + min) * 100;


		slideView (value);
	}

	void slideView(double percentage){

		float newHeight = (float)percentage - 100;

		Vector3 position = transform.localPosition;
		position.y = newHeight;
		transform.localPosition = position;
	}

}
