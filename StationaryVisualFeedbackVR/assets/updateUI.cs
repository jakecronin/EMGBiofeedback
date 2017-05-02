using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEditor;
using System.IO;


public class updateUI : MonoBehaviour {

	public GameObject textObject;
	private Text textToModify;
	public string path;
	public RectTransform transform;

	// Use this for initialization
	void Start () {
		textToModify = textObject.GetComponent<Text> ();
		transform = GetComponent<RectTransform> ();
		//slideView (20);
	}
	
	// Update is called once per frame
	void Update () {
		//read data
		StreamReader sr = new StreamReader (path);
		string data = sr.ReadToEnd ();

		//update string
		textToModify.text = "Percent Force: " + data;

		//convert to double and send to slideView
		double value = double.Parse (data);
		Debug.Log (data);
		slideView (value);
	}

	void slideView(double percentage){

		float newHeight = (float)percentage - 100;

		Vector3 position = transform.localPosition;
		position.y = newHeight;
		transform.localPosition = position;
	}

}
