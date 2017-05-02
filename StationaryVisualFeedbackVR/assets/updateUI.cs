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
	private Text textToModify;
	public string path;
	public RectTransform transform;

	// Use this for initialization
	void Start () {
		textToModify = textObject.GetComponent<Text> ();
		transform = GetComponent<RectTransform> ();
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
		double value = double.Parse (data);
		Debug.Log (data);

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
