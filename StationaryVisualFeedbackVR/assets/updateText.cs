
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;
using UnityEditor;
using UnityEngine.UI;

public class updateText : MonoBehaviour {

	public Text textAsset;
	public string path;

	// Use this for initialization
	void Start () {
		textAsset = GetComponent<Text>();
		if (path == null) {
			path = "AssetsItem/JakeData.txt";
		}
	}

	// Update is called once per frame
	void Update () {
		writeToString(ReadString());
	}

	string ReadString()
	{
		//Read the text from directly from the test.txt file
		StreamReader reader = new StreamReader(path); 
		string data = reader.ReadToEnd ();
		Debug.Log(data);
		reader.Close();
		return data;
	}
	void writeToString(string toAppend){
		textAsset.text = "Percent force: " + toAppend;
	}
}
