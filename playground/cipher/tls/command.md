  ステップ1: Googleの証明書を取得・保存

  # 証明書を取得してファイルに保存
  openssl s_client -connect google.com:443 </dev/null 2>/dev/null \
    | openssl x509 -out google.crt

  # 保存できたか確認
  ls -la google.crt

  ---
  ステップ2: 証明書の中身を読む

  openssl x509 -in google.crt -text -noout

  長いので重要な部分だけ絞って見るなら：

  # 発行者・有効期限・ドメインだけ
  openssl x509 -in google.crt -text -noout \
    | grep -E "Issuer|Subject|Not Before|Not After|DNS:" | head -20

  # サーバの公開鍵だけ
  openssl x509 -in google.crt -pubkey -noout

  ---
  ステップ3: 中間CA証明書を取得

  # 証明書チェーンの2枚目（中間CA）を取得
  openssl s_client -connect google.com:443 -showcerts </dev/null 2>/dev/null \
    | awk '/-----BEGIN CERTIFICATE-----/{c++} c==2{print} /-----END CERTIFICATE-----/ && c==2{exit}' \
    > intermediate.crt

  # 中間CAの名前を確認
  openssl x509 -in intermediate.crt -text -noout | grep "Subject:"

  ---
  ステップ4: 署名を実際に検証

  # 中間CAの公開鍵でGoogleの証明書の署名を検証
  openssl verify -CAfile intermediate.crt google.crt
  # → "google.crt: OK" と出れば成功！

  ---
  ステップ5: 署名のバイト列を見る

  # CAの秘密鍵で暗号化された署名そのもの
  openssl x509 -in google.crt -text -noout | grep -A 20 "Signature Value"

  ---
  ステップ6: 自分でハッシュを計算

  # 証明書データのSHA256ハッシュを自分で計算
  openssl x509 -in google.crt -outform DER | openssl dgst -sha256
