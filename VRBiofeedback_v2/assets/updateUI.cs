using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEditor;
using System.IO;


public class updateUI : MonoBehaviour {
	public float min;
	public float max;
	public GameObject textObject;
	public string path;
	public RectTransform transform;

	public GameObject controllerLeft;

	//used for haptic feedback
	private SteamVR_TrackedObject trackedObject;
	private SteamVR_Controller.Device device;

	//Used for modifying text
	private Text textToModify;

	//used for delegate
	private SteamVR_TrackedController controller;

	// Use this for initialization
	void Start () {

		//load objects
		textToModify = textObject.GetComponent<Text> ();
		transform = GetComponent<RectTransform> ();
		controller = controllerLeft.GetComponent<SteamVR_TrackedController> ();
		controller.TriggerClicked += TriggerPressed;
		trackedObject = controllerLeft.GetComponent<SteamVR_TrackedObject> ();
	}

	void TriggerPressed(object sender, ClickedEventArgs e){
		updateMax ();

		device = SteamVR_Controller.Input((int)trackedObject.index);
		device.TriggerHapticPulse (900);
		Debug.Log ("left trigger pressed");
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
		textToModify.text = "Percent Force: " + data;

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
