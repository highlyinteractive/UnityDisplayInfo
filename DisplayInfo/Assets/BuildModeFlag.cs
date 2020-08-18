using UnityEngine;
using UnityEngine.UI;

[RequireComponent(typeof(Text))]
public class BuildModeFlag : MonoBehaviour
{
	private void Start ()
	{
		Text txt = GetComponent<Text>();
		txt.text = "[BUILD MODE]";
	}
}
